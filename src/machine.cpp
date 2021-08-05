#include "include/machine.h"
#include <string.h>
#include "include/machine_base.h"

bool aresPtrComp(ares_t *a1, ares_t *a2)
{
    return a1->time < a2->time;
}

bool aresComp(ares_t a1, ares_t a2)
{
    return a1.time < a2.time;
}

void machineReset(machine_base_t *base)
{
    machine_t *m = (machine_t *) base->ptr_derived_object;
    machine_base_reset(base);
    m->makespan = 0;
    m->total_completion_time = 0;
    m->tool->time = 0;
    m->wire->time = 0;
}

double setupTimeCWN(job_base_t *_prev, job_base_t *_next)
{
    if (_prev) {
        job_t *prev = (job_t *) _prev->ptr_derived_object;
        job_t *next = (job_t *) _next->ptr_derived_object;
        if (isSameInfo(prev->part_no, next->part_no))
            return 0.0;
        else
            return 30.0;
    }
    return 0;
}

double setupTimeCK(job_base_t *_prev, job_base_t *_next)
{
    if (_prev) {
        job_t *prev = (job_t *) _prev->ptr_derived_object;
        job_t *next = (job_t *) _next->ptr_derived_object;
        if (prev->part_no.data.text[5] == next->part_no.data.text[5]) {
            return 0.0;
        } else {
            return 0;
        }
    }
    return 0;
}

double setupTimeEU(job_base_t *_prev, job_base_t *_next)
{
    if (_next) {
        job_t *next = (job_t *) _next->ptr_derived_object;
        if (next->urgent_code)
            return 0;  // FIXME
        else
            return 0;
    }
    return -1;
}

double setupTimeMCSC(job_base_t *_prev, job_base_t *_next)
{
    if (_prev && _next) {
        job_t *prev = (job_t *) _prev->ptr_derived_object;
        job_t *next = (job_t *) _next->ptr_derived_object;
        if (prev->pin_package.data.number[0] ==
            next->pin_package.data.number[0]) {
            return 84;
        } else {
            return 90;
        }
    }
    return 0;
}

double setupTimeCSC(job_base_t *_prev, job_base_t *_next)
{
    if (_next) {
        job_t *next = (job_t *) _next->ptr_derived_object;
        if (next->part_no.data.text[5] != 'A')
            return 0;
    }
    return 0;
}

double setupTimeUSC(job_base_t *_prev, job_base_t *_next)
{
    if (_next) {
        job_t *next = (job_t *) _next->ptr_derived_object;
        if (next->part_no.data.text[5] != 'A' && next->urgent_code == 'Y')
            return 72;
        else
            return 0;
    }
    return -1;
}

double calculateSetupTime(job_t *prev,
                          job_t *next,
                          machine_base_operations_t *ops)
{
    // for all setup time function
    double time = 0;
    if (isSameInfo(prev->bdid, next->bdid))
        return 0.0;
    for (unsigned int i = 0; i < ops->sizeof_setup_time_function_array; ++i) {
        if (prev)
            time += ops->setup_times[i](&prev->base, &next->base);
        else
            time += ops->setup_times[i](NULL, &next->base);
    }
    return time;
}

void scheduling(machine_t *machine, machine_base_operations_t *ops)
{
    list_ele_t *it;
    job_t *job;
    job_t *prev_job = &machine->current_job;
    it = machine->base.root;
    double arrival_time;
    double setup_time;
    bool hasICSI = false;
    double start_time = machine->base.available_time;
    double total_completion_time = 0;
    int setup_times = 0;
    int setup_times_in1440 = 0;
    machine->setup_times = 0;
    while (it) {
        job = (job_t *) it->ptr_derived_object;
        arrival_time = job->base.arriv_t;
        setup_time = calculateSetupTime(prev_job, job, ops);
        if (setup_time != 0.0)
            ++setup_times;
        if (start_time < 1440) {
            ++setup_times_in1440;
        }
        if (!hasICSI) {
            if (strncmp(job->customer.data.text, "ICSI", 4) == 0) {
                setup_time += 54;
                hasICSI = true;
            }
        }

        start_time = (start_time + setup_time) > arrival_time
                         ? start_time + setup_time
                         : arrival_time;
        set_start_time(&job->base, start_time);
        start_time = get_end_time(&job->base);
        total_completion_time += start_time;
        prev_job = job;
        it = it->next;
    }
    machine->makespan = start_time;
    machine->tool->time = start_time;
    machine->wire->time = start_time;
    machine->total_completion_time = total_completion_time;
    machine->quality = setup_times * 500 + total_completion_time;
    machine->setup_times = setup_times_in1440;
}


void setLastJobInMachine(machine_t *machine)
{
    list_ele_t *it = machine->base.root;
    if(!it)
        return;
    job_t *job;
    while (it) {
        job = (job_t *) it->ptr_derived_object;
        it = it->next;
    }
    machine->current_job = *job;
    machine->current_job.base.ptr_derived_object = &machine->current_job;
    machine->current_job.list.ptr_derived_object = &machine->current_job;
}


void _insertHeadAlgorithm(machine_t *machine, machine_base_operations_t *mbops)
{
    list_ele_t *it = machine->base.root;
    list_ele_t *prev;
    list_ele_t *next;
    list_ele_t *head = it;

    job_t *job;
    double size = -1;

    // search the head segment
    // check if the machine has at least two jobs
    if (it == nullptr || it->next == nullptr)
        return;  // if the machine has only one job -> return immediately

    // get the first segment size
    job = (job_t *) it->ptr_derived_object;
    size = job->base.start_time - machine->base.available_time;

    it = it->next;
    while (it) {
        job = (job_t *) it->ptr_derived_object;
        // check if ptime is smaller then size
        if (job->base.ptime < size &&
            job->base.arriv_t < machine->base.available_time) {
            prev = it->prev;
            next = it->next;
            prev->next = next;
            if (next != nullptr)
                next->prev = prev;

            it->prev = nullptr;
            it->next = head;
            head->prev = it;
            machine->base.root = it;
            break;
        }
        it = it->next;
    }
    scheduling(machine, mbops);
}

void insertAlgorithm(machine_t *machine, machine_base_operations_t *mbops)
{
    _insertHeadAlgorithm(machine, mbops);

    list_ele_t *it = machine->base.root;
    list_ele_t *prev, *next, *it2;
    job_t *it_job, *prev_job, *next_job, *it2_job;
    double size = -1;

    while (it && it->next) {
        it_job = (job_t *) it->ptr_derived_object;
        next_job = (job_t *) it->next->ptr_derived_object;
        size = next_job->base.start_time - it_job->base.end_time;
        if (size > 0) {
            it2 = it->next->next;
            while (it2) {
                it2_job = (job_t *) it2->ptr_derived_object;
                if (it2_job->base.ptime < size &&
                    it2_job->base.arriv_t <= it_job->base.end_time) {
                    prev = it2->prev;
                    next = it2->next;
                    prev->next = next;
                    if (next != nullptr)
                        next->prev = prev;

                    prev = it;
                    next = it->next;
                    prev->next = it2;
                    next->prev = it2;
                    it2->next = next;
                    it2->prev = prev;
                    scheduling(machine, mbops);
                    break;
                }
                it2 = it2->next;
            }
        }
        it = it->next;
    }
}

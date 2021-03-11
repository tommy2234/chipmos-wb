#include <include/job_base.h>
#include <gtest/gtest.h>
class TestJobBase : public testing::Test
{
protected:
    int testSetNumber(int);
	double testSetMsGenePointer(double * ms_gene);
	double testSetOsSeqGenePointer(double * os_seq_gene);
	void testSetProcessTime(ProcessTime **ptime);
	double testSetArrivT(double arriv_time);
	double testSetStartTime(double start_time);
	double testGetMsGene();
	double testGetOsSeqGene();
	unsigned int testGetMachineNo();
	double testGetArrivT();
	double testGetStartTime();
	double testGetEndTime();
    
    
private:
    JobBase jb;
};

int TestJobBase::testSetNumber(int number){
    jb.setNumber(number);
    return jb.number;
}
double TestJobBase::testSetMsGenePointer(double * ms_gene){
	jb.setMsGenePointer(ms_gene);
	return jb.ms_gene;
}
double TestJobBase::testSetOsSeqGenePointer(double * os_seq_gene){
	jb.setOsSeqGenePointer(os_seq_gene);
	return jb.os_seq_gene;
}
ProcessTime** TestJobBase::testSetProcessTime(ProcessTime **ptime){
	jb.setProcessTime(ptime);
	return jb.ptime;
}
double TestJobBase::testSetArrivT(double arriv_time){
	jb.setArrivT(arriv_time);
	return jb.arriv_time;
}
double TestJobBase::testSetStartTime(double start_time){
	jb.setStartTime(start_time)
	return jb.start_time;
}

double TestJobBase::testGetMsGene(){
	return jb.getMsGene(); 
}
double TestJobBase::testGetOsSeqGene(){
	return jb.getOsSeqGene(); 
}
unsigned int TestJobBase::testGetMachineNo(){
	return jb.getMachineNo();
}
double TestJobBase::testGetArrivT(){
	return jb.getArrivT();
}
double TestJobBase::testGetStartTime(){
	return jb.getStartTime();
}
double TestJobBase::testGetEndTime(){
	return jb.getEndTime();
	
}


TEST_F(TestJobBase, test_JobBase_setNumber){
    EXPECT_EQ(testSetNumber(5), 5);
    EXPECT_EQ(testSetNumber(10), 10);
}
TEST_F(TestJobBase, test_JobBase_setMsGenePointer){
    EXPECT_EQ(testSetMsGenePointer(5), 5);
    EXPECT_EQ(testSetMsGenePointer(10), 10);
}
TEST_F(TestJobBase, test_JobBase_setOsSeqGenePointer){
    EXPECT_EQ(testSetOsSeqGenePointer(5), 5);
    EXPECT_EQ(testSetOsSeqGenePointer(10), 10);
}
TEST_F(TestJobBase, test_JobBase_setProcessTime){
    EXPECT_EQ(testSetProcessTime(5), 5);
    EXPECT_EQ(testSetProcessTime(10), 10);
}
TEST_F(TestJobBase, test_JobBase_setArrivT){
    EXPECT_EQ(testSetArrivT(5), 5);
    EXPECT_EQ(testSetArrivT(10), 10);
}
TEST_F(TestJobBase, test_JobBase_setStartTime){
    EXPECT_EQ(testSetStartTime(5), 5);
    EXPECT_EQ(testSetStartTime(10), 10);
}
TEST_F(TestJobBase, test_JobBase_getMsGene){
    EXPECT_EQ(testGetMsGene(), jb.getMsGene());
    EXPECT_EQ(testGetMsGene(), jb.getMsGene());
}
TEST_F(TestJobBase, test_JobBase_getOsSeqGene){
    EXPECT_EQ(testGetOsSeqGene(), jb.getOsSeqGene());
    EXPECT_EQ(testGetOsSeqGene(), jb.getOsSeqGene());
}
TEST_F(TestJobBase, test_JobBase_getMachineNo){
    EXPECT_EQ(testGetMachineNo(), jb.getMachineNo());
    EXPECT_EQ(testGetMachineNo(), jb.getMachineNo());
}
TEST_F(TestJobBase, test_JobBase_getArrivT){
    EXPECT_EQ(testGetArrivT(), jb.getArrivT());
    EXPECT_EQ(testGetArrivT(), jb.getArrivT());
}
TEST_F(TestJobBase, test_JobBase_getStartTime){
    EXPECT_EQ(testGetStartTime(), jb.getStartTime());
    EXPECT_EQ(testGetStartTime(), jb.getStartTime());
}
TEST_F(TestJobBase, test_JobBase_getEndTime){
    EXPECT_EQ(testGetEndTime(), jb.getEndTime());
    EXPECT_EQ(testGetEndTime(),jb.getEndTime());
}







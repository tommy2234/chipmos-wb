#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>

#include "include/csv.h"

#define private public
#define protected public
#include "include/route.h"
#undef private
#undef protected

#include <exception>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace route
{
struct route_test_case_t {
    string test_route_name;
    vector<int> test_opers;
    friend ostream &operator<<(ostream &out, struct route_test_case_t cs)
    {
        out << "route name :" << cs.test_route_name << "->";
        out << ::testing::PrintToString(cs.test_opers);
        return out;
    }
};


class test_route_t : public testing::WithParamInterface<route_test_case_t>,
                     public testing::Test
{
public:
    route_t *route;
    static csv_t *__routelist;
    test_route_t()
    {
        route = new route_t();
        route->setRoute(*test_route_t::__routelist);
    }

    ~test_route_t() { delete route; }

    static void SetUpTestSuite()
    {
        if (__routelist == nullptr) {
            test_route_t::__routelist =
                new csv_t("test_data/route_list.csv", "r", true, true);
            test_route_t::__routelist->setHeaders(
                {{"route", "wrto_route"},
                 {"oper", "wrto_oper"},
                 {"seq", "wrto_seq_num"},
                 {"desc", "wrto_opr_shrt_desc"}});
        }
    }

    static void TearDownTestSuite()
    {
        if (__routelist != nullptr) {
            delete test_route_t::__routelist;
            __routelist = nullptr;
        }
    }
};
csv_t *test_route_t::__routelist = nullptr;  // initialization __routelist


TEST_P(test_route_t, test_if_oper_in_WB_7)
{
    route_test_case_t cs = GetParam();
    set<int> route_list = route->_beforeWB[cs.test_route_name];
    vector<int> &test_oper = cs.test_opers;

    ASSERT_EQ(route_list.size(), test_oper.size())
        << cs << ", route list : " << ::testing::PrintToString(route_list)
        << endl;
    for (int i = 0; i < test_oper.size(); ++i) {
        ASSERT_EQ(route_list.count(test_oper[i]), 1)
            << cs << ", route list : " << ::testing::PrintToString(route_list)
            << endl;
    }
}

INSTANTIATE_TEST_SUITE_P(
    route_list,
    test_route_t,
    testing::Values(
        route_test_case_t{"BGA140",
                          {2050, 2060, 2061, 2070, 2080, 2150, 2090, 2200}},
        route_test_case_t{"BGA145",
                          {2040, 2050, 2060, 2070, 2080, 2150, 2090, 2200}},
        route_test_case_t{"BGA154",
                          {2050, 2060, 2061, 2070, 2080, 2150, 2090, 2200}},
        route_test_case_t{"BGA161",
                          {2040, 2050, 2060, 2070, 2080, 2150, 2090, 2200}},
        route_test_case_t{
            "MBGA138",
            {2070, 2080, 2150, 2090, 2200, 2205, 2209, 2130, 2140, 3150, 3200}},
        route_test_case_t{"MBGA176",
                          {2070, 2080, 2150, 2090, 2200, 2205, 2210, 2130, 3130,
                           3140, 3150, 3200}},
        route_test_case_t{"MBGA222",
                          {2070, 2130, 2080, 2150, 2090, 2200, 2205, 2210, 3130,
                           2140, 3330, 3340, 3150, 3200, 3205, 3210, 4130, 4330,
                           4340, 3350, 3400, 3205, 3210, 4130, 4330, 4340, 3350,
                           3400, 3405, 3410, 4530, 4730, 4740, 3550, 3600}}));
}  // namespace route

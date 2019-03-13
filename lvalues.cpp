#include <vector>


/*

 1. rvalues indicates objects eligible for move operations, while lvalues generally don't.

 2. CLS(CLS&& inst)
 Here it would be perfectly valid to take rhs's address inside Widget's move constructor,
 so rhs it's an lvalue, even though its type is a rvalue reference.
 By similar reasoning, all parameters are lvalues.

 */


/*
 Lvalues si Rvalues parametrii de tip int
 */
namespace lvalues_test_int
{
    using namespace std;
    
    void test_val(int&& a)
    {
        cout << "CALL test_rval(int&& a)\n";
        a++;
    }

    void test_val(int& a)
    {
        cout << "CALL test_lval(int& a)\n";
        a++;
    }

    void run_test()
    {
        int a = 3;
        
        cout << "Initial val: " << a << "\n";
        // out: Initial val: 3
        test_val(a);
        // out: CALL test_lval(int& a)
        cout << "After call val: " << a << "\n";
        // out: After call val: 4
        test_val(std::move(a));
        // out: CALL test_rval(int&& a)
        cout << "After call val: " << a << "\n";
        // out: After call val: 5
    }
}


namespace class_defines
{
#define TEST_RVAL(CLS) \
void test_val(CLS&& wd) { \
cout << "CALL " << __func__ << "(" << STR(CLS) << "&& wd)\n"; }
    
#define TEST_LVAL(CLS) \
void test_val(CLS& wd) { \
cout << "CALL " << __func__ << "(" << STR(CLS) << "& wd)\n"; }
    
    
    using namespace std;
    
#undef CLS
#define CLS Wd_constr_destr
    class CLS
    {
    public:
        CLS_CONSTR(CLS);
        CLS_DESTR(CLS);
    };
    TEST_RVAL(CLS);
    TEST_LVAL(CLS);
    
#undef CLS
#define CLS Wd_constr_destr_del_move_moveassg
    class CLS
    {
    public:
        // there is no default copy constr because there exists deleted move constructor
        // and move assignment operator
        CLS_CONSTR(CLS);
        CLS_DESTR(CLS);
        CLS_MOVE_CONSTR_DEL(CLS);
        CLS_MOVE_ASSIG_DEL(CLS);
    };
    TEST_RVAL(CLS);
    TEST_LVAL(CLS);
    
    
#undef CLS
#define CLS Wd_constr
    class CLS
    {
    public:
        CLS_CONSTR(CLS);
    };
    TEST_RVAL(CLS);
    TEST_LVAL(CLS);
    
    
#undef CLS
#define CLS Wd_constr_destr_move_moveassg
    class CLS
    {
    public:
        // there is no default copy constr because there exists move constructor
        // and move assignment operator
        CLS_CONSTR(CLS);
        CLS_DESTR(CLS);
        CLS_MOVE_CONSTR(CLS);
        CLS_OP_MOVE_ASSIG(CLS);
    };
    TEST_RVAL(CLS);
    TEST_LVAL(CLS);
}

namespace lvalues_test_class
{
    using namespace class_defines;
    
    void run_test()
    {
        {
            // 1. Tipul asta de apel nu ruleaza cu parametru de tip referinta
            // 2. Daca este transmis prin valoare atunci este creat obiectul
            //    apoi copy construcotrul obiectului parametru al functiei,
            //    apoi doi destructorul.
            // 3. Daca este trasmis prin rval atunci obiectul este creat direct
            //    pe stiva functiei (indiferent daca are sau nu move constr).
            //    In cazul asta move constr nu este apelat chiar daca exista.
            test_val(Wd_constr_destr());
            /*
             out: CALL CONSTRUCTOR: Wd_constr_destr_0x7ffeefbff4a8
             out: CALL test_val(Wd_constr_destr&& wd)
             out: CALL DESTRUCTOR: Wd_constr_destr_0x7ffeefbff4a8
             */
        }
        {
            test_val(Wd_constr_destr_del_move_moveassg());
            /*
             out: CALL CONSTRUCTOR: Wd_constr_destr_del_move_moveassg_0x7ffeefbff4a0
             out: CALL test_val(Wd_constr_destr_del_move_moveassg&& wd)
             out: CALL DESTRUCTOR: Wd_constr_destr_del_move_moveassg_0x7ffeefbff4a0
             */
        }
        // out: CALL DESTRUCTOR: Wd_constr_destr_0x7ffeefbff4b8
        {
            Wd_constr wd;
            // out: CALL CONSTRUCTOR: Wd_constr_0x7ffeefbff4a8
            test_val(wd);
            // out: CALL test_val(Wd_constr& wd)
        }
    
        {
            Wd_constr_destr_move_moveassg wd;
            // out: CALL CONSTRUCTOR: Wd_constr_destr_move_moveassg_0x7ffeefbff4a0
            test_val(wd);
            // out: CALL<test_val(Wd_constr_destr_move_moveassg& wd)
        }
        // out: CALL DESTRUCTOR: Wd_constr_destr_move_moveassg_0x7ffeefbff4a0
        {
            test_val(Wd_constr_destr_move_moveassg());
            /*
             out: CALL CONSTRUCTOR: Wd_constr_destr_move_moveassg_0x7ffeefbff498
             out: CALL test_val(Wd_constr_destr_move_moveassg&& wd)
             out: CALL DESTRUCTOR: Wd_constr_destr_move_moveassg_0x7ffeefbff498
             */
        }
        
        {
            /*
             out: CALL CONSTRUCTOR: Wd_constr_destr_move_moveassg_0x7ffeefbff2f8
             out: CALL MOVE CONSTRUCTOR: Wd_constr_destr_move_moveassg_0x100725aa0
             out: CALL DESTRUCTOR: Wd_constr_destr_move_moveassg_0x7ffeefbff2f8
             */
            vector<Wd_constr_destr_move_moveassg> v;
            v.push_back(Wd_constr_destr_move_moveassg());
        }
        // out: CALL DESTRUCTOR: Wd_constr_destr_move_moveassg_0x100725aa0
    }
}

namespace rvalues_test_class
{
    using namespace class_defines;
    
    
}

#include <iostream>
#include <ap_int.h>

/* Khai báo prototype của hàm HLS */
void adder32(ap_uint<32> a,
             ap_uint<32> b,
             ap_uint<32> *sum);

int main()
{
    ap_uint<32> a, b;
    ap_uint<32> sum;

    int error = 0;

    std::cout << "===== HLS ADDER32 TESTBENCH =====" << std::endl;

    /* Test nhiều bộ dữ liệu */
    for (int i = 0; i < 10; i++)
    {
        a = i * 10;
        b = i * 3;

        adder32(a, b, &sum);

        std::cout << "a = " << a
                  << ", b = " << b
                  << ", sum = " << sum << std::endl;

        if (sum != (a + b))
        {
            error = 1;
            std::cout << "❌ ERROR!" << std::endl;
        }
    }

    if (error == 0)
    {
        std::cout << "✅ TEST PASSED" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "❌ TEST FAILED" << std::endl;
        return 1;
    }
}

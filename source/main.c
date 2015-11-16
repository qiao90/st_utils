#include <stdio.h>

void st_mutex_test_intra(void);
void st_event_comsumer_test(void);
void st_event_thread_test(void);
void st_shm_test(void);
void st_tls_test(void);
void st_utils_timer_test(void);

int main(int argc, char* argv)
{
    //st_threadpool_test();
    //st_mutex_test_intra();

    //st_event_comsumer_test();

    //st_event_thread_test();
    //st_tls_test();
    st_utils_timer_test();
}

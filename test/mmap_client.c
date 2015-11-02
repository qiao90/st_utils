#include <stdio.h>

void st_memmap_test2(void);

void st_memmap_test3(void)
{
    P_ST_MEMMAP_T p_token = NULL;
    p_token = st_memmap_open("RPC_SHARE", 1, 1);

    struct mmap_struct* p_st = NULL;

    if ( p_token)
    {
        p_st = (struct mmap_struct*) p_token->location;

        st_print("CHECKING MAP DATA:\n");
        st_print("i:%x\t c:%c\t str:%s\n", p_st->i, p_st->c, p_st->buf);
    }

    st_print("Changing the data from another process!\n");
    p_st->i = 0xAA991;
    p_st->c = 'J';
    strcpy(p_st->buf, "NEW INFO HERE!");

    st_memmap_close(p_token);
}

int main(int argc, char *argv[])
{
    st_memmap_test3();

    return 0;
}

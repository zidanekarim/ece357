struct shared_data_struct
{
    int lock;
    /* Other data structures stuff */
} *ms;
/* signal_handler is the disposition of SIGINT */
void signal_handler(int sig)
{
    spin_lock(&ms->lock);
    /* Manipulate data structure */
    ms->lock = 0;
}
int some_function(int a)
{
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    spin_lock(&ms->lock);
    sigprocmask(SIG_BLOCK, &mask, &oldmask); // now SIGINT is blocked until the lock is released, so handler won't run and deadlock
    /* Manipulate data structure */
    ms->lock = 0;
    sigprocmask(SIG_SETMASK, &oldmask, NULL); // restore old mask
    return something;
}

/* main.c */

#include <zephyr/kernel.h>
#include <zephyr/console/console.h>
#include <zephyr/logging/log.h>

/* Output a greeting message to the console.
 * Returns: (int) 0 on success, != 0 on error.
 */
int greet(void) {
    
    /* write to console */ 
    printk("\nConductor Sensor Emulator\n");

    return 0;

}

int main(void) {

    /* initializations */
    console_init();                         /* initialize CLI */
    int ret;                                /* for return codes */

    /* greet */
    greet();
    
    return 0;
}


/*
 * start be including the appropriate header files 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-features.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * if --enable-minimalist has been turned on, we need to register
 * the support we need so the needed functions aren't removed at compile time
 */
netsnmp_feature_require(long_instance)

/*
 * Then, we declare the variables we want to be accessed 
 */

static FILE *fp;

static char *date;

int my_scalar_date_handler(
     netsnmp_mib_handler          *handler,    // handler stack ptr
     netsnmp_handler_registration *reginfo,    // returned reg ptr
     netsnmp_agent_request_info   *reqinfo,    // request & PDU info
     netsnmp_request_info         *requests    // requests to handle
) {
    char line[250];
    DEBUGMSGTL(("scalar_date",
                "I'm inside the handler"));

    switch(reqinfo->mode) {
        case MODE_GET:
            fp = fopen("current.csv", "r");
            while(fgets(line, sizeof(line), fp) != NULL) {
		if (strncmp(line, "date", strlen("date")) == 0) {
			printf("line matched date\n");
			date = strstr(line, "\t");
			printf("date value:%s\n", date);
                 	break;
		 }
            }
            snmp_set_var_typed_value(requests->requestvb, 
                                     ASN_OCTET_STR,
                                     date,
                                     strlen(date));
            break;
        case MODE_SET_RESERVE1:
        case MODE_SET_RESERVE2:
        case MODE_SET_ACTION:
        case MODE_SET_COMMIT:
        case MODE_SET_UNDO:
        case MODE_SET_FREE:
            break;
        default:
            snmp_log(LOG_ERR, "unknown mode (%d)\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

/*
 * our initialization routine, automatically called by the agent 
 * (to get called, the function name must match init_FILENAME())
 */
void
init_scalar_date(void)
{
    /*
     * the OID we want to register our integer at.  This should be a
     * fully qualified instance.  In our case, it's a scalar     * NET-SNMP-EXAMPLES-MIB::netSnmpExampleInteger.0 (note the
     * trailing 0 which is required for any instantiation of any
     * scalar object) 
     */
    oid             my_registration_oid[] =
        { 1, 3, 6, 1, 4, 1, 777, 1, 5, 0 };


    /*
     * a debugging statement.  Run the agent with -Dexample_scalar_int to see
     * the output of this debugging statement. 
     */
    DEBUGMSGTL(("scalar_date",
                "Initalizing scalar_date.\n"));

/*
location	San Jose / Reid / Hillv, CA, United States (KRHV) 37-20N 121-49W
date	2017-04-26 12:47:00
windspeed	6 MPH
temperature	59
humidity	72%
pressure	1020 hPa
 */

    //netsnmp_handler_registration *file_handler = NULL;
    //Netsnmp_Node_Handler *file_handler = NULL;
    //file_handler =  netsnmp_create_handler_registration("super cool handler",
    //                                                    my_super_cool_handler,
    //                                                    my_registration_oid,
    //                                                    OID_LENGTH(my_registration_oid),
    //                                                    HANDLER_CAN_RWRITE);
    /*
     * the line below registers our "example1" variable above as
     * accessible and makes it writable.  A read only version of the
     * same registration would merely call
     * register_read_only_long_instance() instead.
     * 
     * If we wanted a callback when the value was retrieved or set
     * (even though the details of doing this are handled for you),
     * you could change the NULL pointer below to a valid handler
     * function. 
     */
    netsnmp_register_long_instance("my scalar_date variable",
                                  my_registration_oid,
                                  OID_LENGTH(my_registration_oid),
                                  date, &my_scalar_date_handler);

    DEBUGMSGTL(("scalar_date",
                "Done initalizing scalar_date\n"));
}

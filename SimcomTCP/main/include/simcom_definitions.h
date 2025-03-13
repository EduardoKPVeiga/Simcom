#ifndef SIMCOM_DEFINITIONS_H_
#define SIMCOM_DEFINITIONS_H_

#define SIZE(x) ((sizeof(x)) - 1)

// Gsm application ================================

// Gsm flags and delay
#define ERROR_FLAG_MAX 4

#define DELAY_ERROR_MSG 1000 / portTICK_PERIOD_MS
#define DELAY_MSG 100 / portTICK_PERIOD_MS
#define DELAY_COLD_START 5000 / portTICK_PERIOD_MS
#define DELAY_SLEEP_MODE 100 / portTICK_PERIOD_MS
#define DELAY_CONNECT_TIME 10000 / portTICK_PERIOD_MS
#define DELAY_GNSS_SET_PWR 5000 / portTICK_PERIOD_MS
#define DELAY_DISCONNECT_TIME 1000 / portTICK_PERIOD_MS
#define DELAY_MQTT_IS_CON_TASK 5000 / portTICK_PERIOD_MS
#define DELAY_MQTT_IS_CON_TASK_RETRY 5000 / portTICK_PERIOD_MS

#define DEFAULT_RESET_TIME 12000 / portTICK_PERIOD_MS // ms

#define SIMCOM_PWR_ON_TIMEOUT 30000 // ms
#define SIMCOM_PWR_ON_DELAY 10      // ms
#define SIMCOM_PWR_ON_ATTEMPTS (SIMCOM_PWR_ON_TIMEOUT / SIMCOM_PWR_ON_DELAY)
#define PWRKEY_T_ON 600 / portTICK_PERIOD_MS          // ms
#define PWRKEY_T_ON_UART 2600 / portTICK_PERIOD_MS    // ms
#define PWRKEY_T_ON_STATUS 2600 / portTICK_PERIOD_MS  // ms
#define PWRKEY_T_OFF 1300 / portTICK_PERIOD_MS        // ms
#define PWRKEY_T_OFF_ON 2100 / portTICK_PERIOD_MS     // ms
#define PWRKEY_T_OFF_VDD 1900 / portTICK_PERIOD_MS    // ms
#define PWRKEY_T_OFF_STATUS 5500 / portTICK_PERIOD_MS // ms
#define PWRKEY_T_RESET 12000 / portTICK_PERIOD_MS     // ms

#define GSM_PWR_OFF_TIMEOUT 5000 // ms
#define GSM_PWR_OFF_DELAY 10     // ms
#define GSM_PWR_OFF_ATTEMPTS (SIMCOM_PWR_ON_TIMEOUT / SIMCOM_PWR_ON_DELAY)

#define COORD_ERROR_VALUE INVALID_VALUE
#define GNSS_LOCATION_VARIABLES 3

#define MAX_NUM_CHAR_SEND_BUFF 559
// ================================================

// gsm_commands ===========================================================================
#define BEGIN_CMD "AT"
#define END_CMD "\0\r\n"
#define RESP_DELIMITER "<CR><LF>"
#define CMD_DELIMITER ";"
#define VALUE_DELIMITER ","
#define TEST_CMD "=?"
#define READ_CMD "?"
#define WRITE_CMD "="

#define ECHO_OFF "ATE0"

// SMS
#define CMGF "+CMGF"   // Select SMS message format
#define CMGD "+CMGD"   // Delete SMS message
#define CMGS "+CMGS"   // Send SMS message
#define CMGW "+CMGW"   // Write SMS message to memory
#define CMSS "+CMSS"   // Send SMS message from storage
#define CPOWD "+CPOWD" // Power OFF

#define CNTP "+CNTP" // UTC time

// Responses
#define RESP_OK "OK"
#define RESP_ERROR "ERROR"
#define RESP_DEACTIVE "DEACTIVE"
#define RESP_ACTIVE "ACTIVE"
#define RESP_READY "READY"
#define RESP_NOT_READY "NOT READY"
#define SMS_READY "SMS Ready"
#define ENTER_PSM "ENTER PSM"
#define EXIT_PSM "EXIT PSM"
#define NORMAL_POWER_DOWN "NORMAL POWER DOWN"

#define DELAY_GNSS_LTE_SHIFT 30000 / portTICK_PERIOD_MS

enum pwr_off_enum
{
    URG_PWR_OFF = 0,   // Internet Protocol (IETF STD 5)
    NORMAL_PWR_OFF = 1 // Internet Protocol Version 6
};

enum PDP_type_enum
{
    IP = 0,     // Internet Protocol (IETF STD 5)
    IPV6 = 1,   // Internet Protocol Version 6
    IPV4V6 = 2, // Dual PDN Stack
    NON_IP = 3  // Transfer of Non-IP data to external packet dataNetwork
};

enum D_comp_enum
{
    D_OFF = 0, // Default value
    D_ON = 1,
    V42BIS = 2
};

enum H_comp_enum
{
    H_OFF = 0, // Default value
    H_ON = 1,
    RFC1144 = 2,
    RFC2507 = 3,
    RFC3095 = 4
};

enum Qos_enum
{
    QOS_0 = 0,
    QOS_1 = 1,
    QOS_2 = 2
};

enum CMD_action_enum
{
    WRITE = 0,
    READ = 1,
    TEST = 2,
    EXE = 3
};
// ========================================================================================

// gsm_at_cmd =============================================================================
#define CLBS "+CLBS" // Base station location
#define CFUN "+CFUN" // Set Phone Functionality
#define CPIN "+CPIN" // SIM card status

// Phone function
#define DELAY_CPIN_ATTEMPT 1 / portTICK_PERIOD_MS
#define DELAY_CPIN 5000 / portTICK_PERIOD_MS

enum Cfun_enum
{
    CFUN_MIN_FUNC = 0,
    CFUN_FULL_FUNC = 1,
    CFUN_ERROR = 2
};
// ========================================================================================

// gsm_gnss_cmd ===========================================================================
// GNSS
#define CGNSINF "+CGNSINF"   // GNSS navigation information parsed from NMEA sentences
#define CGNSPWR "+CGNSPWR"   // GNSS power control
#define CGNSMOD "+CGNSMOD"   // GNSS work mode set
#define SGNSCMD "+SGNSCMD"   // GNSS command
#define SGNSERR "SGNSERR"    // SGNSCMD response error
#define CGNSCOLD "+CGNSCOLD" // GNSS cold start
#define CGNSHOT "+CGNSHOT"   // GNSS hot start
#define CGNSWARM "+CGNSWARM" // GNSS warm start

#define INVALID_PARAMETER_VALUE 65535

#define COORD_MULT 1000
#define DEFAULT_LATITUDE -13.000002 * COORD_MULT
#define DEFAULT_LONGITUDE -54.000002 * COORD_MULT
#define DEFAULT_ALTITUDE 16.820 // meters
#define LATITUDE_MULTI 1000000
#define LONGITUDE_MULTI 1000000
#define ALTITUDE_MULTI 100
#define SPEED_MULTI 100

#define GET_GPS_INFO_MAX_ATTEMPTS_INIT 60
#define GET_GPS_INFO_MAX_ATTEMPTS 10
#define GET_GPS_INFO_DELAY 1000 / portTICK_PERIOD_MS
#define GPS_COLD_START_DELAY 60000 / portTICK_PERIOD_MS
#define GPS_HOT_START_DELAY 1000 / portTICK_PERIOD_MS

enum MQTT_status_enum
{
    ERROR = -1,
    OFF = 0,
    ON = 1,
    UNKNOWN = 2
};

enum GNSS_power_mode
{
    GNSS_PWR_MODE_OFF = 0,
    GNSS_PWR_MODE_ON = 1,
    GNSS_PWR_MODE_CMD_ERROR = 2,
    GNSS_PWR_MODE_INVALID = 3
};

enum GNSS_start_mode
{
    HOT_START = 0,
    WARM_START = 1,
    COLD_START = 2
};

enum GNSS_mode_enum
{
    GNSS_OF = 0,
    GNSS_ON = 1,
    GNSS_ON_MULT_INFO = 2
};

enum GNSS_pwrlevel_enum
{
    ALL_TECH = 0,
    ALL_LOW_PWR_TECH = 1,
    ONLY_LOW_AND_MEDIUM_TECH = 2
};

enum GNSS_accuracy_enum
{
    DEFAULT = 0,
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3
};

enum GNSS_error_enum
{
    SUCCESS = 0,
    GENERAL_FAILURE = 1,
    MISSING_CALLBACK = 2,
    INVALID_PARAMETER = 3,
    ID_EXISTS = 4,
    ID_UNKNOWN = 5,
    ALREADY_STARTED = 6,
    NOT_INITIALIZED = 7,
    MAX_GEOFENCES_NUM = 8,
    NOT_SUPPORTED = 9,
    SINGLE_SHOT_TIMEOUT = 10,
    GNSS_ENGINE_NOT_LOADED = 11,
    LICENSE_DISABLE = 12,
    POSITION_INVALID = 13
};

enum GNSS_get_location_failed
{
    GNSS_GET_LOC_ANSWER_NOT_FOUND = 0,
    GNSS_GET_LOC_INVALID_DATA = 1,
    GNSS_GET_LOC_VALID_DATA = 2
};
// ========================================================================================

// gsm_gprs_cmd ===========================================================================
// GPRS
#define SNPING4 "+SNPING4" // Ping IPV4
#define CGDCONT "+CGDCONT" // Define PDP context
// ========================================================================================

// gsm_mqtt_cmd ===========================================================================
// MQTT commands
#define SMCONF "+SMCONF"         // Set MQTT Parameter
#define SMSSL "+SMSSL"           // Select SSL Configure
#define SMCONN "+SMCONN"         // MQTT Connection
#define SMPUB "+SMPUB"           // Send Packet
#define SMSUB "+SMSUB"           // Subscribe Packet
#define SMUNSUB "+SMUNSUB"       // Unsubscribe Packet
#define SMDISC "+SMDISC"         // Disconnection MQTT
#define SMPUB "+SMPUB"           // Send packet
#define SMSTATE "+SMSTATE"       // MQTT connection status
#define SMRCVSLPTM "+SMRCVSLPTM" // Set MQTT thread sleep time

#define CLIENTID "CLIENTID"
#define URL "URL"
#define KEEPTIME "KEEPTIME"
#define USERNAME "USERNAME"
#define PASSWORD "PASSWORD"
#define CLEANSS "CLEANSS"
#define QOS "QOS"
#define TOPIC "TOPIC"
#define MESSAGE "MESSAGE"
#define RETAIN "RETAIN"
#define SUBHEX "SUBHEX"
#define ASYNCMODE "ASYNCMODE"

#define SMCONN_RESP_TRUE "+SMCONN: 1"

// Mqtt connect function
#define DELAY_SMCONN_ATTEMPT 10 / portTICK_PERIOD_MS
#define DELAY_SMCONN 30000 / portTICK_PERIOD_MS

// Mqtt publish function
#define DELAY_SMPUB_ATTEMPT 1 / portTICK_PERIOD_MS
#define DELAY_SMPUB 5000 / portTICK_PERIOD_MS

enum Thread_action_enum
{
    DELETE_TH = 0,
    WRITE_TH = 1
};
// ========================================================================================

// gsm_simcon_cmd =========================================================================
// SIM cmds
#define CNSMOD "+CNSMOD"         // Network system mode
#define CGNAPN "+CGNAPN"         // Network APN
#define CEREG "+CEREG"           // EPS network registration status
#define CREG "+CREG"             // CSD network registration status
#define CGREG "+CGREG"           // GPRS network registration status
#define CAOPEN "+CAOPEN"         // TCP/UDP connection
#define CSQ "+CSQ"               // RF signal
#define CGATT "+CGATT"           // Check PS service
#define COPS "+COPS"             // Query information
#define CNCFG "+CNCFG"           // PDP configure
#define CBANDCFG "+CBANDCFG"     // Test if the band is properly set
#define CNBS "+CNBS"             // Band scan optimization
#define CENG "+CENG"             // Engineering mode
#define CSCLK "+CSCLK"           // Configure slow clock
#define CPSMS "+CPSMS"           // Power saving mode setting
#define CLTS "+CLTS"             // Get local timestamp
#define CBATCHK "+CBATCHK"       // Set VBAT checking feature ON/OFF
#define CNMP "+CNMP"             // Preferred mode selection
#define CREBOOT "+CREBOOT"       // Reboot module
#define CPSMSTATUS "+CPSMSTATUS" // Enable deep sleep wakeup indication
#define CPSMRDP "+CPSMRDP"       // Read PSM Dynamic Parameters
#define CPSMCFGEXT "+CPSMCFGEXT" // Configure modem optimization of PSM
#define CPSMCFG "+CPSMCFG"       // Configure minimum threshold value
#define CFUN "+CFUN"             // Set Phone Functionality
#define CPIN "+CPIN"             // SIM card status

#define CNACT "+CNACT" // APP network active
#define CMNB "+CMNB"   // Preferred selection between CAT-M and NB-IoT

// LTE connection
#define APP_PDP "+APP PDP"
#define DELAY_LTE_CONN_ATTEMPT 1 / portTICK_PERIOD_MS
#define DELAY_LTE_CONN 5000 / portTICK_PERIOD_MS

/**
 * PSM_WAKE_UP_TIMER_UNIT for SIM7070, SIM7080 and SIM7090 series
 *
 * 0 -> 10min
 * 1 -> 1h
 * 2 -> 10h
 * 3 -> 2sec
 * 4 -> 30sec
 * 5 -> 1min
 * 6 -> 320h
 */
#define PSM_WAKE_UP_TIMER_UNIT 0b00000100 // (0, 1, ... , 6)
#define PSM_WAKE_UP_TIMER_MULT 0b00000011
#define PSM_WAKE_UP_TIMER (uint16_t)((uint8_t)(PSM_WAKE_UP_TIMER_UNIT << 5) + PSM_WAKE_UP_TIMER_MULT)

/**
 * REQ_PERIODIC_TAU, requested periodic TAU for SIM7070, SIM7080 and SIM7090 series
 *
 * 0 -> 2sec
 * 1 -> 1min
 * 2 -> 6min
 */
#define REQ_PERIODIC_TAU_UNIT 0b00000000 // (0, 1, ... , 6)
#define REQ_PERIODIC_TAU_MULT 0b00001111
#define REQ_PERIODIC_TAU (uint16_t)((uint8_t)(REQ_PERIODIC_TAU_UNIT << 5) + REQ_PERIODIC_TAU_MULT)

enum Action_enum
{
    DEACTIVED = 0,
    ACTIVED = 1,
    AUTO_ACTIVE = 2
};

enum Connection_mode_enum
{
    AUTO = 2,
    GSM_ONLY = 13,
    LTE_ONLY = 38,
    GSM_AND_LTE = 51
};

enum Network_select_enum
{
    CAT_M = 1,
    NB_IOT = 2,
    BOTH = 3
};
// ========================================================================================

// gsm_tcp_udp_cmd ========================================================================
// TCP/UDP
#define CACFG "+CACFG"         // configure transparent transmission parameters
#define CAOPEN "+CAOPEN"       // Open a TCP/UDP connection
#define CACLOSE "+CACLOSE"     // Close a TCP/UDP connection
#define CASTATE "+CASTATE"     // Query TCP/UDP connection state
#define CASSLCFG "+CASSLCFG"   // SSL configure
#define CASEND "+CASEND"       // Request to send data through TCP/UDP
#define CAACK "+CAACK"         // Query send data information
#define CARECV "+CARECV"       // Receive Data via an Established Connection
#define CADATAIND "+CADATAIND" // Data indication

#define KEEPALIVE "KEEPALIVE"
// ========================================================================================
#endif
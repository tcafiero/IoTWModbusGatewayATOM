//include this same file as contract for all client and all servers into the same line
//BEGIN WARNING this part must be identical for all servers and client on the same SR485 network

#ifndef contract_h
#define contract_h

#define N_OF_SERVERS 32
#define N_OF_SENSORS 10
#define N_OF_ACTUATORS 8

#if 0
enum FunctionCode {
  ANY_FUNCTION_CODE       = 0x00, // Only valid for server to register function codes
  READ_COIL               = 0x01,
  READ_DISCR_INPUT        = 0x02,
  READ_HOLD_REGISTER      = 0x03,
  READ_INPUT_REGISTER     = 0x04,
  WRITE_COIL              = 0x05,
  WRITE_HOLD_REGISTER     = 0x06,
  READ_EXCEPTION_SERIAL   = 0x07,
  DIAGNOSTICS_SERIAL      = 0x08,
  READ_COMM_CNT_SERIAL    = 0x0B,
  READ_COMM_LOG_SERIAL    = 0x0C,
  WRITE_MULT_COILS        = 0x0F,
  WRITE_MULT_REGISTERS    = 0x10,
  REPORT_SERVER_ID_SERIAL = 0x11,
  READ_FILE_RECORD        = 0x14,
  WRITE_FILE_RECORD       = 0x15,
  MASK_WRITE_REGISTER     = 0x16,
  R_W_MULT_REGISTERS      = 0x17,
  READ_FIFO_QUEUE         = 0x18,
  ENCAPSULATED_INTERFACE  = 0x2B,
  USER_DEFINED_41         = 0x41,
  USER_DEFINED_42         = 0x42,
  USER_DEFINED_43         = 0x43,
  USER_DEFINED_44         = 0x44,
  USER_DEFINED_45         = 0x45,
  USER_DEFINED_46         = 0x46,
  USER_DEFINED_47         = 0x47,
  USER_DEFINED_48         = 0x48,
  USER_DEFINED_64         = 0x64,
  USER_DEFINED_65         = 0x65,
  USER_DEFINED_66         = 0x66,
  USER_DEFINED_67         = 0x67,
  USER_DEFINED_68         = 0x68,
  USER_DEFINED_69         = 0x69,
  USER_DEFINED_6A         = 0x6A,
  USER_DEFINED_6B         = 0x6B,
  USER_DEFINED_6C         = 0x6C,
  USER_DEFINED_6D         = 0x6D,
  USER_DEFINED_6E         = 0x6E,
};
#endif

struct contract_struct {
  const int baudrate;
  const char* root;
  const char* plant;
  const char* unit;
  const int num_servers;
  struct {
    const int address;
	const int function;
    const char* name;
    const int num_sensors;
    const int sensors_address;
    struct {
      const char* name;
	  const float factor;
    } sensor[N_OF_SENSORS];
    const int num_actuators;
    const int actuators_address;
    struct {
      const char* name;
    } actuator[N_OF_ACTUATORS];
  } server[N_OF_SERVERS];
};
extern struct contract_struct contract;
#endif

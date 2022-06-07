#ifndef _PA_FIFOMESSAGES
#define _PA_FIFOMESSAGES

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup General
	@{
*/

// The FIFO_SOUND channel is free to use as PAlib doesn't use it...
/// PAlib Fifo channel number...
#define FIFO_PALIB FIFO_SOUND

/// PA_FifoMsg message types.
enum{
	// ARM7 to ARM9 messages
	/// Input message (ARM7->ARM9)
	PA_MSG_INPUT = 0x7000,

	// ARM9 to ARM7 messages
	/// Microphone record message (ARM9->ARM7)
	PA_MSG_MIC = 0x7100,
	/// DS lite screen brightness message (ARM9->ARM7)
	PA_MSG_DSLBRIGHT = 0x7102,
	/// PSG play message (ARM9->ARM7)
	PA_MSG_PSG = 0x7103
};

/// PA_SendFifoCmd() commands.
enum{
	// ARM9 to ARM7 commands
	/// Microphone stop recording message (ARM9->ARM7)
	PA_MSG_MICSTOP = 0x7101
};

/// Represents a message sent through Fifo.
typedef struct{
	/// Type of message.
	u32 type;
	/// --
	union{
		/// Input message data
		struct{
			/// TSC temperature diode 1
			u16 tdiode1;
			/// TSC temperature diode 1
			u16 tdiode2;
			/// TSC computed temperature
			u32 temperature;
			/// TSC battery
			u16 battery;
			/// Microphone volume
			u8 micvol;
			/// Extra byte - used as padding for now
			u8 extra;
		} InputMsg;
		/// Microphone record message data
		struct{
			/// Buffer to record microphone data
			u8* buffer;
			/// Length of the buffer in bytes.
			u32 length;
		} MicMsg;
		/// DS lite brightness message data
		struct{
			/// Brightness of the lights (0-3).
			u8 brightness;
		} DSLBrightMsg;
		/// PSG play message data.
		struct{
			/// Frequency (in hertz)
			u32 freq;
			/// Channel
			u8 chan;
			/// Volume (0-127)
			u8 vol;
			/// Pan (0-64-127)
			u8 pan;
			/// Duty (0-7)
			u8 duty;
		} PSGMsg;
	};
} PA_FifoMsg;

// Macros to ease the sending of fifo data
/// Send a PA_FifoMsg structure to the other CPU.
#define PA_SendFifoMsg(msg) fifoSendDatamsg(FIFO_PALIB, sizeof(PA_FifoMsg), (u8*) &msg)
/// Send a 32bit value to the other CPU.
#define PA_SendFifoVal(val) fifoSendValue32(FIFO_PALIB, val)
/// Send a command value to the other CPU (same as PA_SendFifoVal but for readability).
#define PA_SendFifoCmd PA_SendFifoVal

/// Receive a PA_FifoMsg structure from the other CPU.
#define PA_GetFifoMsg(msg, bytes) fifoGetDatamsg(FIFO_PALIB, bytes, (u8*) &msg)

// The macros below assume the executing CPU hasn't set a Value32 handler.
/// Wait for the other CPU to send a return value.
#define PA_FifoRetWait() while(!fifoCheckValue32(FIFO_PALIB))
/// Get the other CPU's return value.
#define PA_FifoRetVal() fifoGetValue32(FIFO_PALIB)

/// Inline function to ease the getting of the return value (wait + get)
static inline u32 PA_FifoGetRetVal(){
	PA_FifoRetWait();
	return PA_FifoRetVal();
}

/*! @} */

#ifdef __cplusplus
}
#endif

#endif

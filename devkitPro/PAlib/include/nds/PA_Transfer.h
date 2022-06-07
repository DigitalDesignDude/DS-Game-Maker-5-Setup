// PAlib transfer region (appended after the libnds one).

#ifndef _PA_TRANSFER_
#define _PA_TRANSFER_

#ifdef ARM9
#define LEGACY __attribute__ ((deprecated))
#else
#define LEGACY
#endif

/*! \addtogroup General
	@{
*/

/// PAlib transfer region type.
typedef struct {
	// Extra values that aren't read by libnds
	/// TSC temperature diode 1
	vuint16 tdiode1;
	/// TSC temperature diode 2
	vuint16 tdiode2;
	/// TSC computed temperature
	vuint32 temperature;
	/// TSC battery
	vuint16 battery;
	/// Microphone volume
	vuint8 micvol;
	/// Extra field - used as padding for now.
	vuint8 extra;

	/// Legacy IPC field
	LEGACY vuint32 mailData;
} PA_TransferRegion;

/// PAlib transfer region (used for the storage of data coming from the ARM7).
/// libnds also does this. As TransferRegion was removed we just skip the first 256 bytes.
#define PA_Transfer ((volatile PA_TransferRegion*) 0x02FFF100)

/*! @} */

#endif

/*
* UART.cpp
*
*  Created on: Dec 18, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
*/


#if !(defined(_WIN32) || defined(_WIN64))
#error This file must be compiled for WINDOWS target platform. Check the project.
#endif


#include <hal/UART.h>
#include <hal/System.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <windows.h>


using namespace std;
namespace dhcom
{


	class UARTImpl
	{
	private:
		UARTImpl(const System &hw, UART::DEVICE device);
		~UARTImpl();

		inline STATUS setCommParams(UART::BAUDRATE baudRate, UART::PARITY parity, UART::STOPBITS stopBits, UART::FLOWCONTROL flowControl);

		inline STATUS 	open();
		inline STATUS 	close();
		inline bool		isOpen() const;
		inline uint32_t write(const uint8_t *buffer, uint32_t size, STATUS *status);
		inline uint32_t read(uint8_t *buffer, uint32_t size, STATUS *status);

		inline STATUS	setPin(UART::OPIN pin, bool value);
		inline bool		getPin(UART::IPIN pin, STATUS *status) const;

		const char *	deviceName_;
		STATUS			hwStatus_;
		HANDLE			deviceHandle_;

		friend class UART;
	};


	// UART::


	UART::UART(const System &sys, UART::DEVICE device)
		: impl_(new UARTImpl(sys, device))
	{
	}


	UART::~UART()
	{
		delete impl_;
	}


	STATUS UART::setCommParams(UART::BAUDRATE baudRate, UART::PARITY parity, UART::STOPBITS stopBits, UART::FLOWCONTROL flowControl)
	{
		return impl_->setCommParams(baudRate, parity, stopBits, flowControl);
	}


	STATUS UART::open()
	{
		return impl_->open();
	}


	STATUS UART::close()
	{
		return impl_->close();
	}


	bool UART::isOpen() const
	{
		return impl_->isOpen();
	}


	uint32_t UART::write(const uint8_t *buffer, uint32_t size, STATUS *status)
	{
		return impl_->write(buffer, size, status);
	}


	uint32_t UART::read(uint8_t *buffer, uint32_t size, STATUS *status)
	{
		return impl_->read(buffer, size, status);
	}


	STATUS UART::setPin(UART::OPIN pin, bool value)
	{
		return impl_->setPin(pin, value);
	}


	bool UART::getPin(UART::IPIN pin, STATUS *status) const
	{
		return impl_->getPin(pin, status);
	}


	// UARTImpl::


	UARTImpl::UARTImpl(const System &sys, UART::DEVICE device)
		: deviceHandle_(INVALID_HANDLE_VALUE)
	{
		deviceName_ = sys.getUartDeviceName(device, &hwStatus_);
	}


	UARTImpl::~UARTImpl()
	{
		close();
	}


	STATUS UARTImpl::setCommParams(UART::BAUDRATE baudRate, UART::PARITY parity, UART::STOPBITS stopBits, UART::FLOWCONTROL flowControl)
	{
		if(!isOpen())
			return STATUS_DEVICE_NOT_OPEN;

		// retrieve current device control block (DCB)
		DCB dcb;
		if (!GetCommState(deviceHandle_, &dcb))
			return STATUS_DEVICE_CONFIG_FAILED;

		dcb.ByteSize = 8;
		dcb.fBinary = TRUE; 
		dcb.fErrorChar = FALSE;
		dcb.fNull = FALSE;
		dcb.fAbortOnError = FALSE;

		// setting the baud rate
		switch(baudRate)
		{
		case UART::BAUDRATE_110:	
			dcb.BaudRate = CBR_110;
			break;
		case UART::BAUDRATE_300:
			dcb.BaudRate = CBR_300;
			break;
		case UART::BAUDRATE_600:
			dcb.BaudRate = CBR_600;
			break;
		case UART::BAUDRATE_1200:
			dcb.BaudRate = CBR_1200;
			break;
		case UART::BAUDRATE_2400:
			dcb.BaudRate = CBR_2400;
			break;
		case UART::BAUDRATE_4800:
			dcb.BaudRate = CBR_4800;
			break;
		case UART::BAUDRATE_9600:
			dcb.BaudRate = CBR_9600;
			break;
		case UART::BAUDRATE_19200:
			dcb.BaudRate = CBR_19200;
			break;
		case UART::BAUDRATE_38400:
			dcb.BaudRate = CBR_38400;
			break;
		case UART::BAUDRATE_57600:
			dcb.BaudRate = CBR_57600;
			break;
		case UART::BAUDRATE_115200:
			dcb.BaudRate = CBR_115200;
			break;
		}

		// setting the read and write timeouts
		COMMTIMEOUTS ctb;
		memset(&ctb, 0, sizeof (ctb));
        int intervalTimeout = 1000 * 8 / dcb.BaudRate;
        if(intervalTimeout == 0)
            intervalTimeout = 1; // minimal is one millisec
        ctb.ReadTotalTimeoutConstant = ctb.WriteTotalTimeoutConstant = intervalTimeout;
        ctb.ReadIntervalTimeout = intervalTimeout;
        ctb.WriteTotalTimeoutMultiplier = intervalTimeout;
        ctb.ReadTotalTimeoutMultiplier = intervalTimeout;
		if (!SetCommTimeouts(deviceHandle_, &ctb))
			return STATUS_DEVICE_CONFIG_FAILED;

#undef PARITY_NONE
#undef PARITY_EVEN
#undef PARITY_ODD

		// parity
		switch(parity)
		{
		case UART::PARITY_NONE:
			dcb.fParity = FALSE;
			dcb.Parity = NOPARITY;
			break;
		case UART::PARITY_ODD:
			dcb.fParity = TRUE;
			dcb.Parity = ODDPARITY;
			break;
		case UART::PARITY_EVEN:
			dcb.fParity = TRUE;
			dcb.Parity = EVENPARITY;
			break;
		}

		// stop bits 1 or 2
		switch(stopBits)
		{
		case UART::STOPBITS_1:
			dcb.StopBits = ONESTOPBIT;
			break;
		case UART::STOPBITS_2:
			dcb.StopBits = TWOSTOPBITS;
			break;
		}

		// flow control
		switch(flowControl)
		{
		case UART::FLOW_NONE:
			dcb.fOutX = FALSE;			// Disable output X-ON/X-OFF
			dcb.fInX = FALSE;			// Disable input X-ON/X-OFF
			dcb.fOutxCtsFlow = FALSE;	// Disable CTS flow control
			dcb.fOutxDsrFlow = FALSE;	// Disable DSR flow control
			dcb.fRtsControl = RTS_CONTROL_DISABLE; // Turns off RTS line
			dcb.fDtrControl = DTR_CONTROL_ENABLE;
			dcb.fDsrSensitivity = FALSE;
			break;
		case UART::FLOW_RTS_CTS:
			dcb.fOutX = FALSE;				// Disable output X-ON/X-OFF
			dcb.fInX = FALSE;				// Disable input X-ON/X-OFF
			dcb.fOutxCtsFlow = TRUE;		// Enable CTS flow control
			dcb.fOutxDsrFlow = FALSE;
			dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;	// Turns on RTS handshaking
			dcb.fDtrControl = DTR_CONTROL_ENABLE;		// Turns on DTR line during the connection
			dcb.fDsrSensitivity = FALSE;
			break;
		case UART::FLOW_XON_XOFF:
			dcb.fOutX = TRUE;				// Enable output X-ON/X-OFF
			dcb.fInX = TRUE;				// Enable input X-ON/X-OFF
			dcb.fOutxCtsFlow = FALSE;		// Disable CTS flow control
			dcb.fOutxDsrFlow = FALSE;		// Disable DSR flow control
			dcb.fRtsControl = RTS_CONTROL_DISABLE; //Turns off RTS line
			dcb.fDtrControl = DTR_CONTROL_ENABLE;
			dcb.fDsrSensitivity = FALSE;
			break;
		}

		// write the device control block
		if (!SetCommState(deviceHandle_, &dcb))
			return STATUS_DEVICE_CONFIG_FAILED;

		return STATUS_SUCCESS;
	}


	STATUS UARTImpl::open()
	{
		if(hwStatus_)
			return hwStatus_;

		if(isOpen())
			return STATUS_DEVICE_ALREADY_OPEN;

		WCHAR wDeviceName[24];
		mbstowcs_s(NULL, wDeviceName, sizeof(wDeviceName)/sizeof(WCHAR), deviceName_, _TRUNCATE);

		deviceHandle_ = CreateFile( wDeviceName
			,GENERIC_READ | GENERIC_WRITE
			,0
			,NULL
			,OPEN_EXISTING
			,0
			,NULL);

		if(deviceHandle_ == INVALID_HANDLE_VALUE)
			return STATUS_DEVICE_OPEN_FAILED;

		return STATUS_SUCCESS;
	}


	STATUS UARTImpl::close()
	{
		if(isOpen())
		{
			CloseHandle(deviceHandle_);
			deviceHandle_ = INVALID_HANDLE_VALUE;
		}
		return STATUS_SUCCESS;
	}


	bool UARTImpl::isOpen() const
	{
		return deviceHandle_ != INVALID_HANDLE_VALUE;
	}


	uint32_t UARTImpl::write(const uint8_t *buffer, uint32_t size, STATUS *status)
	{
		if(!isOpen())
		{
			if(status) *status = STATUS_DEVICE_NOT_OPEN;
			return 0;
		}

		DWORD bytesSend = 0;
		WriteFile(deviceHandle_, buffer, size, &bytesSend, NULL);
		if(status) *status = STATUS_SUCCESS;
		return bytesSend;
	}


	uint32_t UARTImpl::read(uint8_t *buffer, uint32_t size, STATUS *status)
	{
		if(!isOpen())
		{
			if(status) *status = STATUS_DEVICE_NOT_OPEN;
			return 0;
		}

		DWORD bytesRead = 0;
		ReadFile(deviceHandle_, buffer, size, &bytesRead, NULL);
		if(status) *status = STATUS_SUCCESS;
		return bytesRead;
	}


	STATUS UARTImpl::setPin(UART::OPIN pin, bool value)
	{
		if(!isOpen())
			return STATUS_DEVICE_NOT_OPEN;

		DWORD function;
		switch(pin)
		{
		case UART::OPIN_DTR:
			function = value ? SETDTR : CLRDTR;
			break;

		case UART::OPIN_RTS:
			function = value ? SETRTS : CLRRTS;
			break;
		}

		if(!EscapeCommFunction(deviceHandle_, function))
			return STATUS_DEVICE_WRITE_FAILED;

		return STATUS_SUCCESS;
	}


	// GetCommModemStatus
	bool UARTImpl::getPin(UART::IPIN pin, STATUS *status) const
	{
		if(!isOpen())
		{
			if(status) *status = STATUS_DEVICE_NOT_OPEN;
			return false;
		}

		DWORD pinStates;
		if(!GetCommModemStatus(deviceHandle_, &pinStates))
		{
			if(status) *status = STATUS_DEVICE_READ_FAILED;
			return false;
		}

        bool result;
        STATUS newStatus;
		switch(pin)
		{
		case UART::IPIN_DSR:
            newStatus = STATUS_SUCCESS;
            result = 0 != (pinStates & MS_DSR_ON);
            break;

		case UART::IPIN_CTS:
            newStatus = STATUS_SUCCESS;
            result = 0 != (pinStates & MS_CTS_ON);
            break;

		case UART::IPIN_CD:
            newStatus = STATUS_SUCCESS;
            result = 0 != (pinStates & MS_RLSD_ON);
            break;

		case UART::IPIN_RI:
            newStatus = STATUS_SUCCESS;
            result = 0 != (pinStates & MS_RING_ON);
            break;

        default:
            newStatus = STATUS_DEVICE_READ_FAILED;
            result = false;
            break;
		}

        if(status) *status = newStatus;
        return result;
	}
}

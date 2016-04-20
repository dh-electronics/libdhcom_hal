/*
 * spi.h
 *
 *  Created on: 07.11.2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef SPI_H_
#define SPI_H_


#include <DHCOM_HAL.h>
#include <hal/Types.h>


namespace dhcom
{


class SPIImpl;
class System;


class DHCOM_HAL SPI
{
public:
	/// SPI bus enumeration
	enum DEVICE
	{
        DEVICE_SPI1 = 0,    ///< DHCOM SPI1 is used
        DEVICE_SPI2         ///< DHCOM SPI2 is used
	};

    /// CS signal enumeration (reserved for future, in fact - is ignored because only a single CS is supported by the DHCOM hardware)
	enum CHIPSELECT
	{
		CS_0 = 0, ///< Select spi slave 1
		CS_1 = 1, ///< Select spi slave 2
		CS_2 = 2, ///< Select spi slave 3
	};

	/// SPI mode enumeration
	enum MODE
	{
		MODE_0,	MODE_1,	MODE_2,	MODE_3,
	};

    SPI(DEVICE device, CHIPSELECT chipSelect);  //< uses default system data
    SPI(const System &sys, DEVICE device, CHIPSELECT chipSelect = CS_0);
	~SPI();

	STATUS open();
	STATUS close();
	bool isOpen() const;

	STATUS setCommParams(MODE mode, uint8_t bits, quint32 freqHZ);
	int transceive(const uint8_t *outputBuffer, uint8_t *inputBuffer, quint32 count, STATUS *status = NULL);

private:
    SPI(const SPI &) : impl_(NULL) {}

	SPIImpl * const 	impl_;
};


/** \file
 *  \brief Definitions of SPI buses of DHCOM modules.
 *
 *  \class SPI
 *  \brief SPI master device class.
 *  The class covers communication with SPI slave devices from DHCOM.
 *  The reading and writing happens simultaneously - as the byte is shifted in from the bus,
 *  the other byte is shifted out to the bus.
 *  The class provides no protection from any simultaneous use of the same SPI port, normally this is done by linux kernel.
 */


/** \fn SPI::SPI(SPI::DEVICE device, SPI::CHIPSELECT chipSelect)
 *  \brief Constructs the SPI device object.
 *  \param device - the SPI bus device to open.
 *  \param chipSelect - the slave chip select to use (on DHCOM must be 0 for any of the two SPI buses).
 */


/** \fn SPI::~SPI()
 *  \brief Destructs the SPI device object, closing it if it was open.
 */


/** \fn STATUS SPI::open();
 *  \brief Opens the SPI device for I/O.
 *  \return can return STATUS_DEVICE_ALREADY_OPEN, STATUS_DEVICE_OPEN_FAILED or STATUS_SUCCESS.
 */


/** \fn STATUS SPI::close();
 *  \brief Closes the SPI device.
 *  \return STATUS_SUCCESS.
 */


/** \fn bool SPI::isOpen() const;
 *  \brief Checks if the SPI device is open.
 *  \return true if the device is open.
 */


/** \fn STATUS SPI::setCommParams(SPI::MODE mode, uint8_t bits, quint32 freqHZ);
 *  \brief Sets the communication parameters of the SPI device.
 *  \param mode - the mode of the spi communication, which determines the rules about the phase and polarity of the CLK signal.
 *  \param bits - the number of bits in the SPI word.
 *  \param freqHZ - frequency of SPI communication (in Hertz)
 *  This call also implicitly sets the MSb to come first.
 *  \return can return STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_CONFIG_FAILED or STATUS_SUCCESS.
 */


/** \fn int SPI::transceive(const uint8_t *outputBuffer, uint8_t *inputBuffer, quint32 count, STATUS *status = NULL);
 *  \brief Provides two-way communication over SPI.
 *  \param outputBuffer - the buffer from which the data is taken to be sent.
 *  \param inputBuffer - the buffer into which the incoming data is received.
 *  \param count - the number of bytes in the buffers.
 *  \param status - if NULL, status is not returned, otherwise may be set to STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_WRITE_FAILED or STATUS_SUCCESS.
 *  \return the number of bytes done, or -1 for an error.
 */


}


#endif /* SPI_H_ */

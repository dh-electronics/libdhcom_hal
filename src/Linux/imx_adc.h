// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2013 DH electronics GmbH


#ifndef __IMX_ADC_H__
#define __IMX_ADC_H__

/* Initialize ADC. arg type: none. */
#define ADC_INIT                   _IO('p', 0xb0)
/* De-initialize ADC. arg type: none. */
#define ADC_DEINIT                 _IO('p', 0xb1)
/* Convert one channel. arg type: pointer to t_adc_convert_param. */
#define ADC_CONVERT                _IOWR('p', 0xb2, int)
/* Convert multiple channels. arg type: pointer to t_adc_convert_param. */
#define ADC_CONVERT_MULTICHANNEL   _IOWR('p', 0xb4, int)

/* input channels for IMX25 ADC */
enum t_channel {
	CHANNEL_ADC0=2,
	CHANNEL_ADC1=3,
	CHANNEL_ADC2=4,
	CHANNEL_MULTICHNNEL=5,
};

/*!
 * This structure is used with IOCTL code ADC_CONVERT
 * and ADC_CONVERT_MULTICHANNEL.
 */
struct t_adc_convert_param {
	/* channel or channels to be sampled.  */
	enum t_channel channel;
	/* holds up to 16 sampling results */
	unsigned short result[16];
};

#endif	/* __IMX_ADC_H__ */

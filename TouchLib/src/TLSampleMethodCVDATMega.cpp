/*
 * TLSampleMethodCVD.cpp - Capacitive sensing implementation using CVD method
 * for TouchLibrary for Arduino
 * 
 * https://github.com/AdmarSchoonen/TLSensor
 * Copyright (c) 2016 - 2017 Admar Schoonen
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include "TouchLib.h"
#include "TLSampleMethodCVD.h"
#include "BoardID.h"

#if IS_ATMEGA

#define TL_N_CHARGES_MIN_DEFAULT			1
#define TL_N_CHARGES_MAX_DEFAULT			1

#define TL_CHARGE_DELAY_SENSOR_DEFAULT			0
#define TL_CHARGE_DELAY_ADC_DEFAULT			0

#define TL_ADC_RESOLUTION_BIT					10
#define TL_BAR_LOWER_PCT					40
#define TL_BAR_UPPER_PCT					80

#define TL_ADC_MAX                                              ((1 << TL_ADC_RESOLUTION_BIT) - 1)

bool TLHasMux5(void)
{
	#if IS_ATMEGA128X_256X
	return true;
	#else
	return false;
	#endif
}

void TLSetAdcReferencePin(int pin)
{
	unsigned char mux;

	if (TLHasMux5()) {
		if (pin - A0 < 8) {
			mux = pin - A0;
		} else {
			mux = 0x20 + (pin - A0 - 8);
		}

		ADMUX &= ~0x1F;
		ADMUX |= (mux & 0x1F);
		if (mux > 0x1F) {
			ADCSRB |= 0x08;
		} else {
			ADCSRB &= ~0x08;
		}
	} else {
		mux = pin - A0;
		ADMUX &= ~0x0F;
		ADMUX |= (mux & 0x0F);
	}
}

int TLAnalogRead(int pin)
{
	// Arduino UNO seems to accept both Axx and xx notation but official API documentation uses xx
	return analogRead(pin - A0);
}

#endif
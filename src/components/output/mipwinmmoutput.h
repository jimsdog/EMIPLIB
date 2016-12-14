/*
    
  This file is a part of EMIPLIB, the EDM Media over IP Library.
  
  Copyright (C) 2006-2011  Hasselt University - Expertise Centre for
                      Digital Media (EDM) (http://www.edm.uhasselt.be)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  
  USA

*/

/**
 * \file mipwinmmoutput.h
 */

#ifndef MIPWINMMOUTPUT_H

#define MIPWINMMOUTPUT_H

#include "mipconfig.h"

#ifdef MIPCONFIG_SUPPORT_WINMM

#include "mipcomponent.h"
#include "miptime.h"
#include "mipsignalwaiter.h"
#include <jthread/jmutex.h>
#include <windows.h>
#ifndef _WIN32_WCE
	#include <mmsystem.h>
#endif // _WIN32_WCE

/** Win32/WinCE Soundcard output component.
 *  This component allows playing back sound on Win32 and WinCE platforms.
 *  It uses the MS-Windows Multimedia SDK waveOut... functions. The component
 *  accepts raw audio messages using signed 16 bit little endian encoding.
 *  No messages are generated by the component.
 */
class EMIPLIB_IMPORTEXPORT MIPWinMMOutput : public MIPComponent
{
public:
	MIPWinMMOutput();
	~MIPWinMMOutput();

	/** Open the soundcard playback device.
	 *  This function opens the sound playback device.
	 *  \param sampRate The sampling rate (e.g. 8000, 22050, 44100, ...)
	 *  \param channels The number of channels (e.g. 1 for mono, 2 for stereo)
	 *  \param blockTime Audio data with a length corresponding to this parameter is
	 *                   sent to the soundcard device during each iteration.
	 *  \param bufferTime The component allocates a number of buffers to store 
	 *         audio data in. This parameter specifies how much data these buffers
	 *         can contain, specified as a time interval. Note that this is not the
	 *         amount of buffering introduced by the component.
	 *  \param highPriority If \c true, the thread of the chain in which this component resides
	 *                      will receive the highest priority. This will guarantee a smooth
	 *                      playback.
	 */
	bool open(int sampRate, int channels, MIPTime blockTime = MIPTime(0.020),
	          MIPTime bufferTime = MIPTime(10.0), bool highPriority = false,
		  UINT deviceID = WAVE_MAPPER);

	/** Close the soundcard playback device.
	 *  Using this function, the previously opened device is closed.
	 */
	bool close();

	bool push(const MIPComponentChain &chain, int64_t iteration, MIPMessage *pMsg);
	bool pull(const MIPComponentChain &chain, int64_t iteration, MIPMessage **pMsg);
private:
	static void CALLBACK outputCallback(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD dwParam1, DWORD dwParam2);

	bool m_init;
	HWAVEOUT m_device;
	int m_sampRate;
	int m_channels;
	WAVEHDR *m_frameArray;
	int m_numBlocks;
	size_t m_blockLength, m_blockFrames;
	int m_blockPos, m_blocksInitialized;
	jthread::JMutex m_bufCountMutex;
	int m_bufCount, m_prevBufCount;
	bool m_flushBuffers;
	MIPTime m_prevCheckTime;
	MIPTime m_delay;
	bool m_threadPrioritySet;
};

#endif // MIPCONFIG_SUPPORT_WINMM

#endif // MIPWINMMOUTPUT_H

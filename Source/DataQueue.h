/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2014 Open Ephys

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef DATAQUEUE_H_INCLUDED
#define DATAQUEUE_H_INCLUDED

#include <JuceHeader.h>

struct CircularBufferIndexes
{
	int index1;
	int size1;
	int index2;
	int size2;
};

class DataQueue
{
public:
	DataQueue(int blockSize, int nBlocks);
	~DataQueue();
	void setChannels(int nChans);
	void resize(int nBlocks);
	void getTimestampsForBlock(int idx, Array<int64>& timestamps) const;

	//Only the methods after this comment are considered thread-safe.
	//Caution must be had to avoid calling more than one of the methods above simulatenously
	void writeChannel(const AudioSampleBuffer& buffer, int channel, int nSamples, int64 timestamp);
	bool startRead(Array<CircularBufferIndexes>& indexes, Array<int64>& timestamps, int nMax);
	const AudioSampleBuffer& getAudioBufferReference() const;
	void stopRead();

	int getSampleRate();
	int getNumChannels();

private:
	void fillTimestamps(int channel, int index, int size, int64 timestamp);

	int lastIdx;
	int sample_rate;

	OwnedArray<AbstractFifo> m_fifos;
	AudioSampleBuffer m_buffer;
	Array<int> m_readSamples;
	OwnedArray<Array<int64>> m_timestamps;
	Array<int64> m_lastReadTimestamps;

	int m_numChans;
	const int m_blockSize;
	bool m_readInProgress;
	int m_numBlocks;
	int m_maxSize;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DataQueue);
};


#endif  // DATAQUEUE_H_INCLUDED

// Noise++ Library
// Copyright (c) 2008, Urs C. Hanselmann
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright notice,
//      this list of conditions and the following disclaimer in the documentation
//      and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include "Noise.h"
#include "NoiseUtils.h"

namespace noisepp
{

void Module::write (utils::OutStream &s) const
{
}

void Module::read (utils::InStream &s)
{
}

void PerlinModuleBase::write (utils::OutStream &s) const
{
	s.writeDouble (mFrequency);
	s.writeInt (mOctaveCount);
	s.writeInt (mSeed);
	s.writeInt (mQuality);
	s.writeDouble (mLacunarity);
	s.writeDouble (mPersistence);
	s.writeDouble (mScale);
}

void PerlinModuleBase::read (utils::InStream &s)
{
	mFrequency = s.readDouble ();
	mOctaveCount = s.readInt ();
	mSeed = s.readInt ();
	mQuality = s.readInt ();
	mLacunarity = s.readDouble ();
	mPersistence = s.readDouble ();
	mScale = s.readDouble ();
}

void ClampModule::write (utils::OutStream &s) const
{
	s.writeDouble (mLowerBound);
	s.writeDouble (mUpperBound);
}

void ClampModule::read (utils::InStream &s)
{
	mLowerBound = s.readDouble ();
	mUpperBound = s.readDouble ();
}

void ConstantModule::write (utils::OutStream &s) const
{
	s.writeDouble (mValue);
}

void ConstantModule::read (utils::InStream &s)
{
	mValue = s.readDouble ();
}

void CurveModule::write (utils::OutStream &s) const
{
	int count = (int)mControlPoints.size ();
	s.writeInt (count);
	for (int i=0;i<count;++i)
	{
		s.writeDouble (mControlPoints[i].inValue);
		s.writeDouble (mControlPoints[i].outValue);
	}
}

void CurveModule::read (utils::InStream &s)
{
	mControlPoints.clear ();
	int count = s.readInt ();
	for (int i=0;i<count;++i)
	{
		double inValue = s.readDouble ();
		double outValue = s.readDouble ();
		addControlPoint (inValue, outValue);
	}
}

void ExponentModule::write (utils::OutStream &s) const
{
	s.writeDouble (mExponent);
}

void ExponentModule::read (utils::InStream &s)
{
	mExponent = s.readDouble ();
}

void RidgedMultiModule::write (utils::OutStream &s) const
{
	s.writeDouble (mFrequency);
	s.writeInt (mOctaveCount);
	s.writeInt (mSeed);
	s.writeInt (mQuality);
	s.writeDouble (mLacunarity);
	s.writeDouble (mExponent);
	s.writeDouble (mOffset);
	s.writeDouble (mGain);
	s.writeDouble (mScale);
}

void RidgedMultiModule::read (utils::InStream &s)
{
	mFrequency = s.readDouble ();
	mOctaveCount = s.readInt ();
	mSeed = s.readInt ();
	mQuality = s.readInt ();
	mLacunarity = s.readDouble ();
	mExponent = s.readDouble ();
	mOffset = s.readDouble ();
	mGain = s.readDouble ();
	mScale = s.readDouble ();
}

void ScaleBiasModule::write (utils::OutStream &s) const
{
	s.writeDouble (mScale);
	s.writeDouble (mBias);
}

void ScaleBiasModule::read (utils::InStream &s)
{
	mScale = s.readDouble ();
	mBias = s.readDouble ();
}

void ScalePointModule::write (utils::OutStream &s) const
{
	s.writeDouble (mScaleX);
	s.writeDouble (mScaleY);
	s.writeDouble (mScaleZ);
}

void ScalePointModule::read (utils::InStream &s)
{
	mScaleX = s.readDouble ();
	mScaleY = s.readDouble ();
	mScaleZ = s.readDouble ();
}

void SelectModule::write (utils::OutStream &s) const
{
	s.writeDouble (mLowerBound);
	s.writeDouble (mUpperBound);
	s.writeDouble (mEdgeFalloff);
}

void SelectModule::read (utils::InStream &s)
{
	mLowerBound = s.readDouble ();
	mUpperBound = s.readDouble ();
	mEdgeFalloff = s.readDouble ();
}

void TerraceModule::write (utils::OutStream &s) const
{
	s.writeInt (mInvert);
	int count = (int)mControlPoints.size ();
	s.writeInt (count);
	for (int i=0;i<count;++i)
	{
		s.writeDouble (mControlPoints[i]);
	}
}

void TerraceModule::read (utils::InStream &s)
{
	mControlPoints.clear ();
	mInvert = (s.readInt() != 0);
	int count = s.readInt ();
	for (int i=0;i<count;++i)
	{
		addControlPoint (s.readDouble());
	}
}

void TranslatePointModule::write (utils::OutStream &s) const
{
	s.writeDouble (mTranslationX);
	s.writeDouble (mTranslationY);
	s.writeDouble (mTranslationZ);
}

void TranslatePointModule::read (utils::InStream &s)
{
	mTranslationX = s.readDouble ();
	mTranslationY = s.readDouble ();
	mTranslationZ = s.readDouble ();
}

void TurbulenceModule::write (utils::OutStream &s) const
{
	s.writeDouble (mPower);
	s.writeInt (getRoughness());
	s.writeInt (getSeed());
	s.writeDouble (getFrequency());
	s.writeInt (getQuality());
}

void TurbulenceModule::read (utils::InStream &s)
{
	mPower = s.readDouble ();
	setRoughness (s.readInt());
	setSeed (s.readInt());
	setFrequency (s.readDouble());
	setQuality (s.readInt());
}

void VoronoiModule::write (utils::OutStream &s) const
{
	s.writeDouble (mFrequency);
	s.writeInt (mSeed);
	s.writeDouble (mDisplacement);
	s.writeInt (mEnableDistance);
}

void VoronoiModule::read (utils::InStream &s)
{
	mFrequency = s.readDouble();
	mSeed = s.readInt ();
	mDisplacement = s.readDouble ();
	mEnableDistance = (s.readInt() != 0);
}

};

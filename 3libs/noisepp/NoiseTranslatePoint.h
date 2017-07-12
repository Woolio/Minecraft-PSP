#ifndef NOISEPP_TRANSLATEPOINT_H
#define NOISEPP_TRANSLATEPOINT_H

namespace noisepp
{
	class TranslatePointElement1D : public PipelineElement1D
	{
		private:
			ElementID mElement;
			const PipelineElement1D *mElementPtr;
			Real mTranslationX;

		public:
			TranslatePointElement1D (const Pipeline1D *pipe, ElementID element, Real TranslationX) :
				mElement(element), mTranslationX(TranslationX)
			{
				mElementPtr = pipe->getElement (mElement);
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				return getElementValue (mElementPtr, mElement, x+mTranslationX, cache);
			}

	};

	class TranslatePointElement2D : public PipelineElement2D
	{
		private:
			ElementID mElement;
			const PipelineElement2D *mElementPtr;
			Real mTranslationX;
			Real mTranslationY;

		public:
			TranslatePointElement2D (const Pipeline2D *pipe, ElementID element, Real TranslationX, Real TranslationY) :
				mElement(element), mTranslationX(TranslationX), mTranslationY(TranslationY)
			{
				mElementPtr = pipe->getElement (mElement);
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				return getElementValue (mElementPtr, mElement, x+mTranslationX, y+mTranslationY, cache);
			}

	};

	class TranslatePointElement3D : public PipelineElement3D
	{
		private:
			ElementID mElement;
			const PipelineElement3D *mElementPtr;
			Real mTranslationX;
			Real mTranslationY;
			Real mTranslationZ;

		public:
			TranslatePointElement3D (const Pipeline3D *pipe, ElementID element, Real TranslationX, Real TranslationY, Real TranslationZ) :
				mElement(element), mTranslationX(TranslationX), mTranslationY(TranslationY), mTranslationZ(TranslationZ)
			{
				mElementPtr = pipe->getElement (mElement);
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				return getElementValue (mElementPtr, mElement, x+mTranslationX, y+mTranslationY, z+mTranslationZ, cache);
			}

	};

	/** Transform module for translating.
		Transforms the source module by translating the coordinates.
	*/
	class TranslatePointModule : public Module
	{
		private:
			Real mTranslationX;
			Real mTranslationY;
			Real mTranslationZ;

		public:
			/// Constructor.
			TranslatePointModule() : Module(1), mTranslationX(0.0), mTranslationY(0.0), mTranslationZ(0.0)
			{
			}
			/// Sets the translation applied to the x coordintate.
			void setTranslationX (Real v)
			{
				mTranslationX = v;
			}
			/// Returns the translation applied to the x coordintate.
			Real getTranslationX () const
			{
				return mTranslationX;
			}
			/// Sets the translation applied to the y coordintate.
			void setTranslationY (Real v)
			{
				mTranslationY = v;
			}
			/// Returns the translation applied to the y coordintate.
			Real getTranslationY () const
			{
				return mTranslationY;
			}
			/// Sets the translation applied to the z coordintate.
			void setTranslationZ (Real v)
			{
				mTranslationZ = v;
			}
			/// Returns the translation applied to the z coordintate.
			Real getTranslationZ () const
			{
				return mTranslationZ;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline1D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new TranslatePointElement1D(pipe, first, mTranslationX));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline2D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new TranslatePointElement2D(pipe, first, mTranslationX, mTranslationY));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline3D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new TranslatePointElement3D(pipe, first, mTranslationX, mTranslationY, mTranslationZ));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_TRANSLATEPOINT; }
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};
};

#endif // NOISEPP_TRANSLATEPOINT_H

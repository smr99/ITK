/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkTernaryAddImageFilter_h
#define itkTernaryAddImageFilter_h

#include "itkTernaryGeneratorImageFilter.h"
#include "itkArithmeticOpsFunctors.h"

namespace itk
{

/**
 * \class TernaryAddImageFilter
 * \brief Pixel-wise addition of three images.
 *
 * This class is templated over the types of the three
 * input images and the type of the output image.
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * \ingroup IntensityImageFilters
 * \ingroup ITKImageIntensity
 */
template <typename TInputImage1, typename TInputImage2, typename TInputImage3, typename TOutputImage>
class ITK_TEMPLATE_EXPORT TernaryAddImageFilter
  : public TernaryGeneratorImageFilter<TInputImage1, TInputImage2, TInputImage3, TOutputImage>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(TernaryAddImageFilter);

  /** Standard class type aliases. */
  using Self = TernaryAddImageFilter;
  using Superclass = TernaryGeneratorImageFilter<TInputImage1, TInputImage2, TInputImage3, TOutputImage>;

  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  using FunctorType = Functor::Add3<typename TInputImage1::PixelType,
                                    typename TInputImage2::PixelType,
                                    typename TInputImage3::PixelType,
                                    typename TOutputImage::PixelType>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** \see LightObject::GetNameOfClass() */
  itkOverrideGetNameOfClassMacro(TernaryAddImageFilter);

protected:
  TernaryAddImageFilter()
  {
#if !defined(ITK_WRAPPING_PARSER)
    Superclass::SetFunctor(FunctorType());
#endif
  }
  ~TernaryAddImageFilter() override = default;
};
} // end namespace itk

#endif

/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImageMapper.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef __itkImageMapper_h
#define __itkImageMapper_h

#include "itkImage.h"
#include "itkPoint.h"
#include "itkRegistrationMapper.h"
#include "itkLinearInterpolateImageFunction.h"

namespace itk
{

/** \class ImageMapper
 * \brief Maps values from the image using a transformation
 *
 * This Class is templated over the image type and the 
 * transformation type. Using a point in space, it transforms
 * the point and gets the value of the image at that new point 
 * using some interpolation strategy.
 *
 */



template <class TImage, class TTransformation>
class ITK_EXPORT ImageMapper : 
  public RegistrationMapper< TImage, TTransformation > 

{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef ImageMapper<TImage,TTransformation>  Self;

  /**
   * Standard "Superclass" typedef.
   */

  typedef RegistrationMapper< TImage, TTransformation >  Superclass;

  /** 
   * Smart pointer typedef support 
   */
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;


  /**
   * Typedef of the Point used to represent coordinates
   */
   typedef typename TTransformation::PointType   PointType;


   /**
   * Typedef of transformation parameters
   */
   typedef typename TTransformation::ParametersType   ParametersType;

  /**
   *  Pointer type for the Reference 
   */
  typedef typename Superclass::DomainPointer DomainPointer;

  /**
   *  type for the Reference 
   */
  typedef typename Superclass::DomainType     DomainType;



  /**
   * Typedef of the Image type
   */
   typedef  TImage   ImageType;


  /**
   * Typedef of the Pixel type
   */
   typedef  typename   TImage::PixelType   PixelType;



  /**
   * Typedef of the image index
   */
   typedef  typename   TImage::IndexType   IndexType;



  /**
   * Typedef of the image region
   */
   typedef  typename   TImage::RegionType   RegionType;


  /**
   * Typedef of the image size
   */
   typedef  typename   TImage::SizeType   SizeType;
  
  /**
   * Type of the interpolation function
   */
   typedef LinearInterpolateImageFunction<ImageType>  InterpolatorType;


  /**
   * Type of the interpolation function
   */
   typedef typename  InterpolatorType::Pointer  InterpolatorPointer;


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(ImageMapper, RegistrationMapper);


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);


  /**
   * Set Domain
   */
  void SetDomain( DomainType * domain );



  enum { SpaceDimension = TTransformation::ParametersDimension };

  

public: 

  /**
   * Evaluate the pixel value for the point previously 
   * specified in the IsInside() method
   *
   * \sa IsInside()
   *
   */
   double Evaluate( void ) const; 

   /**
    * Test whether the specified point is inside of
    * the Image Domain. Point coordinates are stored
    * in internal memory to be used by the Evaluate() method.
    *
    * \sa Evaluate();
    *
    */
   bool IsInside( const PointType & point );

protected:

  const double *            m_Spacing;
  const double *            m_Origin;
  IndexType                 m_Start;
  SizeType                  m_Size;
  InterpolatorPointer       m_Interpolator;
  mutable PointType         m_CurrentPoint;

  ImageMapper();
  ~ImageMapper(){};
  ImageMapper(const Self&) {}
  void operator=(const Self&) {}

};

} // end namespace itk



#ifndef ITK_MANUAL_INSTANTIATION
#include <itkImageMapper.txx>
#endif

#endif








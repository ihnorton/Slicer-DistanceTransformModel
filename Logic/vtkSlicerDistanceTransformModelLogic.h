/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// .NAME vtkSlicerDistanceTransformModelLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerDistanceTransformModelLogic_h
#define __vtkSlicerDistanceTransformModelLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes
#include <vtkMRMLModelNode.h>
#include <vtkMRMLModelDisplayNode.h>
#include <vtkMRMLVolumeNode.h>
#include <vtkMRMLScalarVolumeNode.h>

// vtkITK includes
#include <vtkITKDistanceTransform.h>

// vtk includes
#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <vtkImageChangeInformation.h>
#include <vtkImageResample.h>
#include <vtkMarchingCubes.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>

// STD includes
#include <cstdlib>

#include "vtkSlicerDistanceTransformModelModuleLogicExport.h"


/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_DISTANCETRANSFORMMODEL_MODULE_LOGIC_EXPORT vtkSlicerDistanceTransformModelLogic :
  public vtkSlicerModuleLogic
{
public:

  static vtkSlicerDistanceTransformModelLogic *New();
  vtkTypeMacro(vtkSlicerDistanceTransformModelLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);

  /// Initialize listening to MRML events
  void InitializeEventListeners();

  /// MRML events
  void ProcessMRMLEvents(vtkObject *caller, unsigned long event, void *callData);

  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();

  void SetInputLabelVolumeNodeID(const char*);
  void SetOutputModelNodeID(const char*);
  void SetDistanceMapVolumeNodeID(const char*);
  void SetResolution(double);
  void SetDistance(double);

  void UpdateModelPipeline();
  void UpdateDistanceMapVolume();

protected:
  vtkSlicerDistanceTransformModelLogic();
  virtual ~vtkSlicerDistanceTransformModelLogic();

private:
  bool CheckDistanceMapNodes();
  bool CheckModelNodes();

  vtkSlicerDistanceTransformModelLogic(const vtkSlicerDistanceTransformModelLogic&); // Not implemented
  void operator=(const vtkSlicerDistanceTransformModelLogic&);               // Not implemented

  vtkMRMLModelNode* OutputModelNode;
  vtkMRMLVolumeNode* InputLabelVolumeNode;
  vtkMRMLVolumeNode* DistanceMapVolumeNode;
  
  vtkImageChangeInformation* changeIn;
  vtkImageChangeInformation* changeOut;
  vtkImageCast* cast;
  vtkImageResample* resample;
  vtkMarchingCubes* cubes;
  vtkTransformPolyDataFilter* polyTransformFilter;

  vtkMatrix4x4* ijkToRAS;
  vtkITKDistanceTransform* DistanceTransformer;

  const char* OutputModelNodeID;
  const char* LabelMapVolumeNodeID;
  const char* DistanceMapVolumeNodeID;
  double minFactor;
  double Distance;
  double Resolution;

};

#endif

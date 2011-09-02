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

#ifndef __qSlicerDistanceTransformModelModuleWidget_h
#define __qSlicerDistanceTransformModelModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerDistanceTransformModelModuleExport.h"

class qSlicerDistanceTransformModelModuleWidgetPrivate;
class vtkMRMLNode;
class vtkMRMLVolumeNode;
class vtkMRMLModelNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_DISTANCETRANSFORMMODEL_EXPORT qSlicerDistanceTransformModelModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerDistanceTransformModelModuleWidget(QWidget *parent=0);
  virtual ~qSlicerDistanceTransformModelModuleWidget();

public slots:
  void setInputLabelVolumeNode(vtkMRMLNode*);
  void setInputLabelVolumeNode(vtkMRMLVolumeNode*);
  void setDistanceTransformVolumeNode(vtkMRMLNode*);
  void setDistanceTransformVolumeNode(vtkMRMLVolumeNode*);
  void setOutputModelNode(vtkMRMLNode*);
  void setOutputModelNode(vtkMRMLModelNode*);
  void setDistance(double);
  void setResolution(double);
  void runDistanceMap();
//  void runModelPipeline();

protected:
  QScopedPointer<qSlicerDistanceTransformModelModuleWidgetPrivate> d_ptr;
  
  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerDistanceTransformModelModuleWidget);
  Q_DISABLE_COPY(qSlicerDistanceTransformModelModuleWidget);
};

#endif

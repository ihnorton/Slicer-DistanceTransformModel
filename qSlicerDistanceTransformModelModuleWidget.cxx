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

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicerDistanceTransformModelModuleWidget.h"
#include "ui_qSlicerDistanceTransformModelModule.h"

//MRML includes
#include <vtkMRMLModelNode.h>
#include <vtkMRMLScalarVolumeNode.h>

//Logic includes
#include <vtkSlicerDistanceTransformModelLogic.h>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerDistanceTransformModelModuleWidgetPrivate: public Ui_qSlicerDistanceTransformModelModule
{
public:
  qSlicerDistanceTransformModelModuleWidgetPrivate();

  vtkSlicerDistanceTransformModelLogic* logic;
};

//-----------------------------------------------------------------------------
// qSlicerDistanceTransformModelModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerDistanceTransformModelModuleWidgetPrivate::qSlicerDistanceTransformModelModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerDistanceTransformModelModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerDistanceTransformModelModuleWidget::qSlicerDistanceTransformModelModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerDistanceTransformModelModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerDistanceTransformModelModuleWidget::~qSlicerDistanceTransformModelModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerDistanceTransformModelModuleWidget::setup()
{
  Q_D(qSlicerDistanceTransformModelModuleWidget);
  d->setupUi(this);
  
  QObject::connect(d->InputLabelVolumeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)), this, 
                                             SLOT(setInputLabelVolumeNode(vtkMRMLNode*)));
  QObject::connect(d->DistanceMapVolumeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)), this, 
                                             SLOT(setDistanceTransformVolumeNode(vtkMRMLNode*)));
  QObject::connect(d->DistanceMapRun, SIGNAL(clicked()), this,
                                             SLOT(runDistanceMap()) );
  QObject::connect(d->OutputModelSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)), this, 
                                             SLOT(setOutputModelNode(vtkMRMLNode*)));
  QObject::connect(d->ResolutionSlider, SIGNAL(valueChanged(double)), this, 
                                             SLOT(setResolution(double)));
  QObject::connect(d->DistanceSlider, SIGNAL(valueChanged(double)), this, 
                                             SLOT(setDistance(double)));
}

void qSlicerDistanceTransformModelModuleWidget::setInputLabelVolumeNode(vtkMRMLNode* node)
{
  this->setInputLabelVolumeNode(vtkMRMLScalarVolumeNode::SafeDownCast(node));
}

void qSlicerDistanceTransformModelModuleWidget::setInputLabelVolumeNode(vtkMRMLVolumeNode* volumeNode)
{
  Q_D(qSlicerDistanceTransformModelModuleWidget);
  Q_UNUSED(d);
  vtkSlicerDistanceTransformModelLogic* logic = 
    vtkSlicerDistanceTransformModelLogic::SafeDownCast(this->logic());
  if (logic && volumeNode)
    {
    logic->SetInputLabelVolumeNodeID(volumeNode->GetID());
    }
}

void qSlicerDistanceTransformModelModuleWidget::setDistanceTransformVolumeNode(vtkMRMLNode* volumeNode)
{
  this->setDistanceTransformVolumeNode(vtkMRMLScalarVolumeNode::SafeDownCast(volumeNode));
}

void qSlicerDistanceTransformModelModuleWidget::setDistanceTransformVolumeNode(vtkMRMLVolumeNode* volumeNode)
{
  Q_D(qSlicerDistanceTransformModelModuleWidget);
  vtkSlicerDistanceTransformModelLogic* logic = 
    vtkSlicerDistanceTransformModelLogic::SafeDownCast(this->logic());
  if (logic && volumeNode)
    {
    logic->SetDistanceMapVolumeNodeID(volumeNode->GetID());
    d->MapModelBox->setEnabled(true);
    }
  else
    d->MapModelBox->setEnabled(false);
}

void qSlicerDistanceTransformModelModuleWidget::setOutputModelNode(vtkMRMLNode* node)
{
  Q_D(qSlicerDistanceTransformModelModuleWidget);
  Q_UNUSED(d);
  this->setOutputModelNode(vtkMRMLModelNode::SafeDownCast(node));
}

void qSlicerDistanceTransformModelModuleWidget::setOutputModelNode(vtkMRMLModelNode* modelNode)
{
  vtkSlicerDistanceTransformModelLogic* logic = 
    vtkSlicerDistanceTransformModelLogic::SafeDownCast(this->logic());
  if (logic && modelNode)
    {
    logic->SetOutputModelNodeID(modelNode->GetID());
    }
}

void qSlicerDistanceTransformModelModuleWidget::setDistance(double distance)
{
  std::cout << "ui call distance: " << distance << std::endl;
  vtkSlicerDistanceTransformModelLogic* logic = 
    vtkSlicerDistanceTransformModelLogic::SafeDownCast(this->logic());
  if (logic)
    {
    logic->SetDistance(distance);
    }
}

void qSlicerDistanceTransformModelModuleWidget::setResolution(double resolution)
{
  vtkSlicerDistanceTransformModelLogic* logic = 
    vtkSlicerDistanceTransformModelLogic::SafeDownCast(this->logic());
  if (logic)
    {
    logic->SetResolution(resolution);
    }
}

void qSlicerDistanceTransformModelModuleWidget::runDistanceMap()
{
  vtkSlicerDistanceTransformModelLogic* logic = 
    vtkSlicerDistanceTransformModelLogic::SafeDownCast(this->logic());
  if (logic)
    {
    logic->UpdateDistanceMapVolume();
    }
}

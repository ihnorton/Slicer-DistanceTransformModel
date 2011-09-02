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

// ModuleTemplate includes
#include "vtkSlicerDistanceTransformModelLogic.h"

// MRML includes

// VTK includes
#include <vtkNew.h>


//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerDistanceTransformModelLogic);

//----------------------------------------------------------------------------
vtkSlicerDistanceTransformModelLogic::vtkSlicerDistanceTransformModelLogic()
{
  this->InputLabelVolumeNode = NULL;
  this->Distance = 1;
  this->Resolution = .1;
  this->DistanceTransformer = vtkITKDistanceTransform::New();
  this->cubes = vtkMarchingCubes::New();
  this->polyTransformFilter = vtkTransformPolyDataFilter::New();
  this->cast = vtkImageCast::New();
  this->changeIn = vtkImageChangeInformation::New();
  this->changeOut = vtkImageChangeInformation::New();
  this->resample = vtkImageResample::New();
  this->ijkToRAS = vtkMatrix4x4::New();
}

//----------------------------------------------------------------------------
vtkSlicerDistanceTransformModelLogic::~vtkSlicerDistanceTransformModelLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerDistanceTransformModelLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerDistanceTransformModelLogic::InitializeEventListeners()
{
  vtkNew<vtkIntArray> events;
 // events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
 // events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
 // events->InsertNextValue(vtkCommand::ModifiedEvent);
 // events->InsertNextValue(vtkMRMLScene::SceneClosedEvent);
  this->SetAndObserveMRMLSceneEventsInternal(this->GetMRMLScene(), events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerDistanceTransformModelLogic::RegisterNodes()
{
  if(!this->GetMRMLScene())
    {
    return;
    }

}

//-----------------------------------------------------------------------------
void vtkSlicerDistanceTransformModelLogic::ProcessMRMLEvents(
  vtkObject* vtkNotUsed(caller), unsigned long vtkNotUsed(event), void * vtkNotUsed(callData))
{
}

void vtkSlicerDistanceTransformModelLogic::SetInputLabelVolumeNodeID(const char* id)
{
  this->LabelMapVolumeNodeID = id;
}

void vtkSlicerDistanceTransformModelLogic::SetDistanceMapVolumeNodeID(const char* id)
{
  this->DistanceMapVolumeNodeID = id;
  vtkMRMLScalarVolumeNode* volumeNode = vtkMRMLScalarVolumeNode::SafeDownCast(
    this->GetMRMLScene()->GetNodeByID(id));
  this->DistanceMapVolumeNode = volumeNode;
}

bool vtkSlicerDistanceTransformModelLogic::CheckDistanceMapNodes()
{
  return
    this->GetMRMLScene()->GetNodeByID(this->LabelMapVolumeNodeID) &&
    this->GetMRMLScene()->GetNodeByID(this->DistanceMapVolumeNodeID);
}

void vtkSlicerDistanceTransformModelLogic::UpdateDistanceMapVolume()
{
  if (!this->CheckDistanceMapNodes())
    return;
  
  std::cout << "enter UpdateMapPipeline" << std::endl;
  vtkMRMLScalarVolumeNode* volumeNode = 
    vtkMRMLScalarVolumeNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(this->LabelMapVolumeNodeID));
  if (!volumeNode)
    {
    std::cout<< "failed downcast" << std::endl;
    return;
    }
  if (volumeNode->GetImageData() == NULL)
    {
    std::cout << "no image data" << std::endl;
    return;
    }

  vtkMatrix4x4* ijkToRAS = vtkMatrix4x4::New();
  volumeNode->GetIJKToRASMatrix(ijkToRAS);
  this->ijkToRAS->DeepCopy(ijkToRAS);

  this->changeIn->SetInput(volumeNode->GetImageData());
  this->changeIn->SetOutputSpacing(volumeNode->GetSpacing());
  this->cast->SetInput(this->changeIn->GetOutput());
  this->cast->SetOutputScalarTypeToFloat();
  this->DistanceTransformer->SetInput(this->cast->GetOutput());
  this->DistanceTransformer->SetSquaredDistance(0);
  this->DistanceTransformer->SetUseImageSpacing(1);
  this->DistanceTransformer->SetInsideIsPositive(0);
  this->changeOut->SetInput(this->DistanceTransformer->GetOutput());
  this->changeOut->SetOutputSpacing(1,1,1);
  
  vtkMRMLVolumeNode* outputVolumeNode = vtkMRMLVolumeNode::SafeDownCast(
      this->GetMRMLScene()->GetNodeByID(this->DistanceMapVolumeNodeID));
  if (outputVolumeNode)    
    {
    outputVolumeNode->SetAndObserveImageData(this->changeOut->GetOutput());
    outputVolumeNode->SetIJKToRASMatrix(this->ijkToRAS);
    }
  this->UpdateModelPipeline();
  std::cout << "exit UpdateMapPipeline" << std::endl;
}

bool vtkSlicerDistanceTransformModelLogic::CheckModelNodes()
{
  return
    this->GetMRMLScene()->GetNodeByID( this->DistanceMapVolumeNodeID ) &&
    this->GetMRMLScene()->GetNodeByID( this->OutputModelNodeID );
}
  

void vtkSlicerDistanceTransformModelLogic::UpdateModelPipeline()
{
  std::cout << "enter UpdateModelPipeline" << std::endl;
  
  if (!this->DistanceMapVolumeNode && !this->DistanceMapVolumeNode->GetImageData())
    {
    std::cout << "no dmapvol or imagedata" << std::endl;
    return;
    }
  
  std::cout << "res:  " << this->Resolution << std::endl;
  std::cout << "dist: " << this->Distance << std::endl;

  this->resample->SetInput(this->DistanceMapVolumeNode->GetImageData());
  this->resample->SetAxisMagnificationFactor(0,this->Resolution);
  this->resample->SetAxisMagnificationFactor(1,this->Resolution);
  this->resample->SetAxisMagnificationFactor(2,this->Resolution);

  this->cubes->SetInput(this->resample->GetOutput());
  this->cubes->SetValue(0,this->Distance);
  this->polyTransformFilter->SetInput(this->cubes->GetOutput());
  vtkTransform* polyTransform = vtkTransform::New();
  polyTransform->Identity();
  polyTransform->Concatenate(this->ijkToRAS);
  
  this->polyTransformFilter->SetTransform(polyTransform);

  double* spacing = new double[3];
  double* invSpacing = new double[3];
  this->DistanceMapVolumeNode->GetSpacing(spacing);
  for (int i = 0; i<3; i++)
    {
    invSpacing[i] = 1.0 / spacing[i];
    }

  polyTransform->Scale(invSpacing);
  std::cout << "polyTransformFilter update start" << std::endl;
  polyTransformFilter->Update();
  std::cout << "polyTransformFilter update end" << std::endl;
  
  vtkMRMLModelNode* modelNode = vtkMRMLModelNode::SafeDownCast(
      this->GetMRMLScene()->GetNodeByID(this->OutputModelNodeID));
  if(modelNode)
    {
    if(!modelNode->GetDisplayNode())
      {
      vtkMRMLModelDisplayNode* dispNode = vtkMRMLModelDisplayNode::SafeDownCast(
        this->GetMRMLScene()->CreateNodeByClass("vtkMRMLModelDisplayNode") );
      dispNode->SetColor(0.5,1,1);
      this->GetMRMLScene()->AddNode(dispNode);
      modelNode->SetAndObserveDisplayNodeID(dispNode->GetID());
      }
    this->GetMRMLScene()->AddNode(modelNode);
    modelNode->SetAndObservePolyData(polyTransformFilter->GetOutput()); 
    }
  std::cout << "exit UpdateModelPipeline" << std::endl;
}

void vtkSlicerDistanceTransformModelLogic::SetOutputModelNodeID(const char* id)
{
    this->OutputModelNodeID = id;
}

void vtkSlicerDistanceTransformModelLogic::SetResolution(double resolution)
{
  this->Resolution = resolution;
  this->UpdateModelPipeline();
}

void vtkSlicerDistanceTransformModelLogic::SetDistance(double distance)
{
  std::cout << "SetDistance: distance" << std::endl;
  this->Distance = distance;
  this->UpdateModelPipeline();
}

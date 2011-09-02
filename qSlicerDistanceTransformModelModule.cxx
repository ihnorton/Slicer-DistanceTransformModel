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
#include <QtPlugin>

// ExtensionTemplate Logic includes
#include <vtkSlicerDistanceTransformModelLogic.h>

// ExtensionTemplate includes
#include "qSlicerDistanceTransformModelModule.h"
#include "qSlicerDistanceTransformModelModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerDistanceTransformModelModule, qSlicerDistanceTransformModelModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerDistanceTransformModelModulePrivate
{
public:
  qSlicerDistanceTransformModelModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerDistanceTransformModelModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerDistanceTransformModelModulePrivate::qSlicerDistanceTransformModelModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerDistanceTransformModelModule methods

//-----------------------------------------------------------------------------
qSlicerDistanceTransformModelModule::qSlicerDistanceTransformModelModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerDistanceTransformModelModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerDistanceTransformModelModule::~qSlicerDistanceTransformModelModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerDistanceTransformModelModule::helpText()const
{
  return "This DistanceTransformModel module illustrates how a loadable module should "
      "be implemented.";
}

//-----------------------------------------------------------------------------
QString qSlicerDistanceTransformModelModule::acknowledgementText()const
{
  return "This work was supported by ...";
}

//-----------------------------------------------------------------------------
QIcon qSlicerDistanceTransformModelModule::icon()const
{
  return QIcon(":/Icons/DistanceTransformModel.png");
}

//-----------------------------------------------------------------------------
void qSlicerDistanceTransformModelModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerDistanceTransformModelModule::createWidgetRepresentation()
{
  return new qSlicerDistanceTransformModelModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerDistanceTransformModelModule::createLogic()
{
  return vtkSlicerDistanceTransformModelLogic::New();
}

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

#ifndef __qSlicerDistanceTransformModelModule_h
#define __qSlicerDistanceTransformModelModule_h

// SlicerQt includes
#include "qSlicerLoadableModule.h"

#include "qSlicerDistanceTransformModelModuleExport.h"

class qSlicerDistanceTransformModelModulePrivate;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_DISTANCETRANSFORMMODEL_EXPORT qSlicerDistanceTransformModelModule :
  public qSlicerLoadableModule
{
  Q_OBJECT
  Q_INTERFACES(qSlicerLoadableModule);

public:

  typedef qSlicerLoadableModule Superclass;
  explicit qSlicerDistanceTransformModelModule(QObject *parent=0);
  virtual ~qSlicerDistanceTransformModelModule();

  qSlicerGetTitleMacro(QTMODULE_TITLE);
  
  /// Help to use the module
  virtual QString helpText()const;

  /// Return acknowledgements
  virtual QString acknowledgementText()const;
  
  /// Return a custom icon for the module
  virtual QIcon icon()const;

protected:

  /// Initialize the module. Register the volumes reader/writer
  virtual void setup();

  /// Create and return the widget representation associated to this module
  virtual qSlicerAbstractModuleRepresentation * createWidgetRepresentation();

  /// Create and return the logic associated to this module
  virtual vtkMRMLAbstractLogic* createLogic();

protected:
  QScopedPointer<qSlicerDistanceTransformModelModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerDistanceTransformModelModule);
  Q_DISABLE_COPY(qSlicerDistanceTransformModelModule);

};

#endif

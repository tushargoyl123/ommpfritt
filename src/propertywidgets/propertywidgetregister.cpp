#include "propertywidgets/propertywidget.h"
#include "propertywidgets/boolpropertywidget/boolpropertywidget.h"
#include "propertywidgets/colorpropertywidget/colorpropertywidget.h"
#include "propertywidgets/numericpropertywidget/numericpropertywidget.h"
#include "propertywidgets/stringpropertywidget/stringpropertywidget.h"
#include "propertywidgets/transformationpropertywidget/transformationpropertywidget.h"
#include "propertywidgets/referencepropertywidget/referencepropertywidget.h"
#include "propertywidgets/optionspropertywidget/optionspropertywidget.h"

namespace omm
{

void register_propertywidgets()
{
#define REGISTER_PROPERTYWIDGET(TYPE) AbstractPropertyWidget::register_type<TYPE>(#TYPE);

  REGISTER_PROPERTYWIDGET(BoolPropertyWidget);
  REGISTER_PROPERTYWIDGET(ColorPropertyWidget);
  REGISTER_PROPERTYWIDGET(FloatPropertyWidget);
  REGISTER_PROPERTYWIDGET(IntegerPropertyWidget);
  REGISTER_PROPERTYWIDGET(ReferencePropertyWidget);
  REGISTER_PROPERTYWIDGET(StringPropertyWidget);
  REGISTER_PROPERTYWIDGET(TransformationPropertyWidget);
  REGISTER_PROPERTYWIDGET(OptionsPropertyWidget);

#undef REGISTER_PROPERTYWIDGET
}

}  // namespace omm

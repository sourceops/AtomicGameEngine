
#include <ThirdParty/TurboBadger/tb_widgets.h>

#include "../IO/Log.h"
#include "../Input/Input.h"
#include "../Input/InputEvents.h"

#include "UI.h"
#include "UIEvents.h"
#include "UIWidget.h"
#include "UILayout.h"
#include "UIFontDescription.h"
#include "UITextField.h"
#include "UIImageWidget.h"
#include "UISelectList.h"
#include "UIDragDrop.h"
#include "UIDragObject.h"

using namespace tb;

namespace Atomic
{

UIDragDrop::UIDragDrop(Context* context) : Object(context)
{
    SubscribeToEvent(E_MOUSEMOVE, HANDLER(UIDragDrop,HandleMouseMove));
    SubscribeToEvent(E_MOUSEBUTTONUP, HANDLER(UIDragDrop,HandleMouseUp));
    SubscribeToEvent(E_MOUSEBUTTONDOWN, HANDLER(UIDragDrop,HandleMouseDown));

    SharedPtr<UIFontDescription> fd(new UIFontDescription(context));
    fd->SetId("Vera");
    fd->SetSize(12);

    dragText_ = new UITextField(context);
    dragText_->SetFontDescription(fd);
    dragText_->SetGravity(WIDGET_GRAVITY_TOP);

    dragLayout_ = new UILayout(context);
    dragLayout_->AddChild(dragText_);

    dragLayout_->SetLayoutSize(UI_LAYOUT_SIZE_PREFERRED);
    dragLayout_->SetVisibility(UI_WIDGET_VISIBILITY_GONE);

    // put into hierarchy so we aren't pruned
    TBWidget* root = GetSubsystem<UI>()->GetRootWidget();
    root->AddChild(dragLayout_->GetInternalWidget());

}

UIDragDrop::~UIDragDrop()
{

}

void UIDragDrop::HandleMouseDown(StringHash eventType, VariantMap& eventData)
{
    using namespace MouseButtonDown;

    Input* input = GetSubsystem<Input>();

    if (!input->IsMouseVisible())
        return;

    if ((eventData[P_BUTTONS].GetUInt() & MOUSEB_LEFT) && TBWidget::hovered_widget)
    {
        // see if we have a widget
        TBWidget* tbw = TBWidget::hovered_widget;

        while(tbw && !tbw->GetDelegate())
        {
            tbw = tbw->GetParent();
        }

        if (!tbw)
            return;

        UIWidget* widget = (UIWidget*) tbw->GetDelegate();

        if (widget->GetType() == UISelectList::GetTypeStatic())
        {
            // handle select drag
            LOGINFOF("DRAG Select: %s", widget->GetTypeName().CString());
        }
        else
        {
            dragObject_ = widget->GetDragObject();
        }

    }

}

void UIDragDrop::HandleMouseUp(StringHash eventType, VariantMap& eventData)
{
    using namespace MouseButtonUp;

    if (dragObject_.Null())
        return;

    Input* input = GetSubsystem<Input>();

    if (!input->IsMouseVisible())
        return;

    if ((eventData[P_BUTTON].GetInt() ==  MOUSEB_LEFT) &&
            TBWidget::hovered_widget && TBWidget::hovered_widget->GetDelegate())
    {
        UIWidget* widget = (UIWidget*) TBWidget::hovered_widget->GetDelegate();

        VariantMap dropData;
        dropData[DragEnded::P_TARGET] = widget;
        dropData[DragEnded::P_DRAGOBJECT] = dragObject_;
        SendEvent(E_DRAGENDED, dropData);
    }

    // clean up
    dragObject_ = 0;
    dragLayout_->SetVisibility(UI_WIDGET_VISIBILITY_GONE);

}

void UIDragDrop::HandleMouseMove(StringHash eventType, VariantMap& eventData)
{
    if (dragObject_.Null())
        return;

    Input* input = GetSubsystem<Input>();

    if (!input->IsMouseVisible())
        return;

    // initialize if necessary
    if (dragLayout_->GetVisibility() == UI_WIDGET_VISIBILITY_GONE)
    {
        dragLayout_->GetInternalWidget()->SetZ(WIDGET_Z_TOP);
        dragLayout_->SetVisibility(UI_WIDGET_VISIBILITY_VISIBLE);
        dragText_->SetText(dragObject_->GetText());

        UIPreferredSize* sz = dragLayout_->GetPreferredSize();
        dragLayout_->SetRect(IntRect(0, 0, sz->GetMinWidth(), sz->GetMinHeight()));
    }

    using namespace MouseMove;

    int x = eventData[P_X].GetInt();
    int y = eventData[P_Y].GetInt();

    dragLayout_->SetPosition(x, y - 20);

}

}

#include <TurboBadger/tb_widgets.h>
#include <TurboBadger/tb_widgets_common.h>

#include <Atomic/IO/Log.h>
#include <Atomic/Graphics/Texture.h>

#include "UIEvents.h"
#include "UI.h"
#include "UITextureWidget.h"

using namespace tb;

namespace Atomic
{

// internal wiget, as we need to override paint
class TBTextureWidget : public tb::TBWidget
{
    friend class UITextureWidget;

public:
    // For safe typecasting
    TBOBJECT_SUBCLASS(UITextureWidget, tb::TBWidget);

    TBTextureWidget();

    virtual void OnPaint(const PaintProps &paint_props);

private:

    WeakPtr<UITextureWidget> uiTextureWidget_;
    PODVector<float> vertexData_;

};


UITextureWidget::UITextureWidget(Context* context, bool createWidget) : UIWidget(context, false)
{
    if (createWidget)
    {
        widget_ = new TBTextureWidget();
        ((TBTextureWidget*)widget_)->uiTextureWidget_ = this;
        widget_->SetDelegate(this);
        GetSubsystem<UI>()->WrapWidget(this, widget_);
    }
}

UITextureWidget::~UITextureWidget()
{

}

void UITextureWidget::SetTexture(Texture *texture)
{
    texture_ = texture;
}

Texture* UITextureWidget::GetTexture()
{
    return texture_;
}

bool UITextureWidget::OnEvent(const tb::TBWidgetEvent &ev)
{
    return UIWidget::OnEvent(ev);
}

TBTextureWidget::TBTextureWidget()
{
    vertexData_.Resize(6 * UI_VERTEX_SIZE);
    float color;
    ((unsigned&)color) = 0xFFFFFFFF;

    float* data = &vertexData_[0];

    data[2] = 0; data[3] = color; data[4] = 0; data[5] = 0;
    data[8] = 0; data[9] = color; data[10] = 1; data[11] = 0;
    data[14] = 0; data[15] = color; data[16] = 1; data[17] = 1;
    data[20] = 0; data[21] = color; data[22] = 0; data[23] = 0;
    data[26] = 0; data[27] = color; data[28] = 1; data[29] = 1;
    data[32] = 0; data[33] = color; data[34] = 0; data[35] = 1;
}


void TBTextureWidget::OnPaint(const PaintProps &paint_props)
{
    if (uiTextureWidget_.Null() || !uiTextureWidget_->GetTexture())
        return;

    TBRect rect = GetRect();
    rect.x = rect.y = 0;
    ConvertToRoot(rect.x, rect.y);

    float* data = &vertexData_[0];

    data[0] = rect.x;
    data[1] = rect.y;

    data[6] = rect.x + rect.w;
    data[7] =  rect.y;

    data[12] = rect.x + rect.w;
    data[13] = rect.y + rect.h;

    data[18] = rect.x;
    data[19] = rect.y;

    data[24] = rect.x + rect.w;
    data[25] = rect.y + rect.h;

    data[30] = rect.x;
    data[31] = rect.y + rect.h;

    uiTextureWidget_->GetSubsystem<UI>()->SubmitBatchVertexData(uiTextureWidget_->GetTexture(), vertexData_);

}

}
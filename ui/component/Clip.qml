import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import FluentUI

Rectangle {
    id:control
    color: "#00000000"
    layer.enabled: !FluTools.isSoftware()
    layer.effect: OpacityMask{
        maskSource: ShaderEffectSource{
            sourceItem: Rectangle{
                radius: control.radius
                width: control.width
                height: control.height
            }
        }
    }
}

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: suggestionsPage
    title: qsTr("选题推荐 / Topic Suggestions")
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 16
        
        Label { text: qsTr("选题推荐页面 / Topic Suggestions Page") }
        Label { text: qsTr("[待实现功能：AI 生成选题建议]\n[To be implemented: AI-generated topic suggestions]") }
    }
}

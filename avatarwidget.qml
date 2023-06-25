import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15

Item {
    property string name
    property string avatarImage
    property string deadImage
    property int totalTime
    property int singleTime

    enum LabelPosition { Left, Right }
    enum AvatarType { Normal, Dead }

    signal timeout

    Rectangle {
        id: root
        width: 200
        height: 100
        color: "#E9ECEF"

        ColumnLayout {
            anchors.fill: parent
            spacing: 2

            Rectangle {
                id: avatar
                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: 60
                Layout.preferredHeight: 60
                color: "transparent"

                Image {
                    id: avatarImage
                    anchors.fill: parent
                    source: root.avatarImage
                }

                Image {
                    id: deadImage
                    anchors.fill: parent
                    source: root.deadImage
                    visible: false
                }

                Rectangle {
                    id: progressBar
                    anchors.fill: parent
                    color: "transparent"
                    Shape {
                        ShapePath {
                            id: progressBarPath
                            strokeWidth: 6
                            strokeColor: "#18BC9C"
                            fillColor: "transparent"
                            PathArc {
                                x: progressBar.width / 2
                                y: progressBar.height / 2
                                radiusX: progressBar.width / 2 - progressBarPath.strokeWidth / 2
                                radiusY: progressBar.height / 2 - progressBarPath.strokeWidth / 2
                                startAngle: 90
                                sweepAngle: 360
                            }
                        }
                    }

                    Timer {
                        id: timer
                        interval: 10
                        repeat: true
                        running: false
                        property int currentTime: root.singleTime

                        onTriggered: {
                            currentTime -= interval;
                            var newTime = root.totalTime - currentTime;
                            lblLeftTime.text = newTime.toString("mm:ss");
                            if (currentTime === 0) {
                                timeout();
                            }
                            if (currentTime < 0.25 * root.singleTime && avatarBlinkAnim.state !== Animation.Running) {
                                avatarBlinkAnim.start();
                            }
                            var progress = currentTime / root.singleTime;
                            progressBarPath.sweepAngle = progress * 360;
                        }
                    }

                    Rectangle {
                        id: progressBarClip
                        width: progressBar.width
                        height: progressBar.height
                        color: "white"
                        clip: true

                        Shape {
                            ShapePath {
                                strokeWidth: 6
                                fillColor: "transparent"
                                PathArc {
                                    x: progressBarClip.width / 2
                                    y: progressBarClip.height / 2
                                    radiusX: progressBarClip.width / 2 - progressBarPath.strokeWidth / 2
                                    radiusY: progressBarClip.height / 2 - progressBarPath.strokeWidth / 2
                                    startAngle: 90
                                    sweepAngle: 360
                                }
                            }
                        }
                    }
                }

                Image {
                    id: avatarBlink
                    anchors.fill: parent
                    source: ""
                    visible: false
                    Behavior on visible {
                        PropertyAnimation {
                            duration: 600
                            loops: Animation.Infinite
                            to: "#DC3545"
                        }
                    }
                }

                SequentialAnimation on avatarImage.visible {
                    loops: Animation.Infinite
                    PauseAnimation {
                        duration: 2000
                    }
                    PropertyAction {
                        target: avatarImage
                        property: "visible"
                        value: false
                    }
                    PauseAnimation {
                        duration: 500
                    }
                    PropertyAction {
                        target: deadImage
                        property: "visible"
                        value: true
                    }
                    PauseAnimation {
                        duration: 1000
                    }
                    PropertyAction {
                        target: deadImage
                        property: "visible"
                        value: false
                    }
                    PropertyAction {
                        target: avatarImage
                        property: "visible"
                        value: true
                    }
                }

                NumberAnimation on progressBar.sweepAngle {
                    duration: root.singleTime
                    easing.type: Easing.Linear
                    from: 0
                    to: 360
                }

                NumberAnimation on progressBarClip.width {
                    duration: root.singleTime
                    easing.type: Easing.Linear
                    from: progressBar.width
                    to: 0
                }
            }

            Rectangle {
                id: infoLayout
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                Text {
                    id: lblName
                    text: root.name
                    font.pixelSize: 15
                    color: "#1E1E1E"
                }

                Rectangle {
                    id: lblLeftTime
                    Layout.alignment: Qt.AlignRight
                    width: 60
                    height: parent.height
                    color: lblLeftTime.isActive ? "#18BC9C" : "#E9ECEF"

                    Text {
                        anchors.centerIn: parent
                        text: root.totalTime.toString("mm:ss")
                        font.pixelSize: 15
                        color: "#1E1E1E"
                    }
                }
            }
        }

        Connections {
            target: timer
            onTimeout: {
                root.timeout();
            }
        }
    }
}

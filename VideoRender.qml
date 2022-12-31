import QtQuick 2.14
import QtMultimedia 5.12
import QtQuick.Layouts 1.12

VideoOutput {
    id: videoOutput
    fillMode: VideoOutput.PreserveAspectCrop
    source: camera
    Camera {
        id: camera
        captureMode: Camera.CaptureStillImage
    }
}

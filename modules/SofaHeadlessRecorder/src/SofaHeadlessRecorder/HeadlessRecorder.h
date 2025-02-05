/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program. If not, see <http://www.gnu.org/licenses/>.              *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_GUI_HEADLESSRECORDER_H
#define SOFA_GUI_HEADLESSRECORDER_H

#include <sofa/gui/BaseGUI.h>

#include <sofa/simulation/fwd.h>
#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/visual/DrawToolGL.h>
#include <SofaBaseVisual/InteractiveCamera.h>
#include <sofa/core/ObjectFactory.h>

#include <signal.h>

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <memory>

// OPENGL
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glx.h>

// SCREENSHOT
#include <sofa/helper/io/Image.h>
#include <sofa/helper/system/SetDirectory.h>

#include <sofa/helper/gl/VideoRecorderFFMPEG.h>
#include <sofa/helper/gl/Capture.h>

namespace sofa
{

namespace gui
{

namespace hRecorder
{

enum class RecordMode { wallclocktime, simulationtime, timeinterval };

class VideoRecorderFFmpeg;

class HeadlessRecorder : public sofa::gui::BaseGUI
{

public:
    typedef sofa::core::visual::VisualParams VisualParams;
    typedef sofa::core::visual::DrawToolGL   DrawToolGL;

    HeadlessRecorder();
    ~HeadlessRecorder();

    int mainLoop() override;

    void step();
    void redraw() override;
    void resetView();
    void saveView();
    void initializeGL();
    void paintGL();
    void setScene(sofa::simulation::NodeSPtr scene, const char* filename=NULL, bool temporaryFile=false) override;
    void newView();

    // Virtual from BaseGUI
    virtual sofa::simulation::Node* currentSimulation() override;
    virtual int closeGUI() override;
    virtual void setViewerResolution(int width, int height) override;

    // Needed for the registration
    static BaseGUI* CreateGUI(const char* name, sofa::simulation::NodeSPtr groot = NULL, const char* filename = NULL);
    static int RegisterGUIParameters(ArgumentParser* argumentParser);
    static void parseRecordingModeOption();

    static int recordTimeInSeconds; // public for SIGTERM
    static bool recordUntilStopAnimate; // public for SIGTERM

private:
    void record();
    bool canRecord();
    bool keepFrame();

    void displayOBJs();
    void drawScene();
    void calcProjection();

    VisualParams* vparams;
    DrawToolGL   drawTool;

    sofa::simulation::NodeSPtr groot;
    std::string sceneFileName;
    sofa::component::visualmodel::BaseCamera::SPtr currentCamera;

    sofa::helper::gl::VideoRecorderFFMPEG m_videorecorder;
    int m_nFrames;

    GLuint fbo;
    GLuint rbo_color, rbo_depth;
    double lastProjectionMatrix[16];
    double lastModelviewMatrix[16];
    bool initTexturesDone;
    bool initVideoRecorder;
    sofa::helper::gl::Capture m_screencapture;

    static GLsizei width, height;
    static unsigned int fps;
    static std::string fileName;
    static bool saveAsScreenShot, saveAsVideo;
    static HeadlessRecorder instance;
    static std::string recordTypeRaw;
    static RecordMode recordType;
    static float skipTime;
};

} // namespace hRecorder

} // namespace gui

} // namespace sofa

#endif

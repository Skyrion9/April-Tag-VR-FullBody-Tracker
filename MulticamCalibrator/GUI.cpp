#include "GUI.h"

#include <sstream>
#include <string>

#include <opencv2/videoio/registry.hpp>

namespace {

void addTextWithTooltip(wxWindow* parent, wxSizer* sizer, const wxString& label, const wxString& tooltip)
{
    wxStaticText* textObject = new wxStaticText(parent, -1, label);
    textObject->SetToolTip(tooltip);
    sizer->Add(textObject);
}

} // namespace

GUI::GUI(const wxString& title, Parameters * params, Connection* conn)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(650, 650))
{
    wxNotebook* nb = new wxNotebook(this, -1, wxPoint(-1, -1),
        wxSize(-1, -1), wxNB_TOP);

    CameraPage* panel = new CameraPage(nb,this,params);
    ParamsPage* panel2 = new ParamsPage(nb, params, conn);
    LicensePage* panel3 = new LicensePage(nb);

    nb->AddPage(panel, params->language.TAB_CAMERA);
    nb->AddPage(panel2, params->language.TAB_PARAMS);
    nb->AddPage(panel3, params->language.TAB_LICENSE);

    Centre();
}

LicensePage::LicensePage(wxNotebook* parent)
    :wxPanel(parent)
{
    wxNotebook* nb = new wxNotebook(this, -1, wxPoint(-1, -1),
        wxSize(-1, -1), wxNB_TOP);

    // Add 2 pages to the wxNotebook widget
    wxTextCtrl* textCtrl1 = new wxTextCtrl(nb, wxID_ANY, ATT_LICENSE, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    nb->AddPage(textCtrl1, "Tracking software");
    wxTextCtrl* textCtrl2 = new wxTextCtrl(nb, wxID_ANY, APRILTAG_LICENSE, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    nb->AddPage(textCtrl2, "Apriltag library");
    wxTextCtrl* textCtrl3 = new wxTextCtrl(nb, wxID_ANY, OPENCV_LICENSE, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    nb->AddPage(textCtrl3, "OpenCV");

    wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
    bs->Add(nb,1, wxEXPAND);
    this->SetSizer(bs);


    //nb->AddPage(panel, "ATT");

    Centre();
}

CameraPage::CameraPage(wxNotebook* parent,GUI* parentGUI, Parameters* params)
    :wxPanel(parent)
{
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    wxFlexGridSizer* fgs = new wxFlexGridSizer(2, 20, 20);

    wxButton* btn1 = new wxButton(this, GUI::CAMERA_BUTTON, params->language.CAMERA_START_CAMERA);
    wxButton* btn2 = new wxButton(this, GUI::CAMERA_CALIB_BUTTON, params->language.CAMERA_CALIBRATE_CAMERA);
    wxButton* btn4 = new wxButton(this, GUI::CONNECT_BUTTON, params->language.CAMERA_CONNECT);
    wxButton* btn3 = new wxButton(this, GUI::TRACKER_CALIB_BUTTON, params->language.CAMERA_CALIBRATE_TRACKERS);
    wxButton* btn5 = new wxButton(this, GUI::START_BUTTON, params->language.CAMERA_START_DETECTION);

    wxCheckBox* cb1 = new wxCheckBox(this, GUI::CAMERA_CHECKBOX, params->language.CAMERA_PREVIEW_CAMERA,
        wxPoint(20, 20));
    wxCheckBox* cb2 = new wxCheckBox(this, GUI::CAMERA_CALIB_CHECKBOX, params->language.CAMERA_PREVIEW_CALIBRATION,
        wxPoint(20, 20));
    wxCheckBox* cb6 = new wxCheckBox(this, GUI::DISABLE_OUT_CHECKBOX, params->language.CAMERA_DISABLE_OUT,
        wxPoint(20, 20));
    wxCheckBox* cb7 = new wxCheckBox(this, GUI::DISABLE_OPENVR_API_CHECKBOX, params->language.CAMERA_DISABLE_OPENVR_API,
        wxPoint(20, 20));
    wxCheckBox* cb8 = new wxCheckBox(this, GUI::PRIVACY_MODE, params->language.CAMERA_PRIVACY_MODE,
        wxPoint(20, 20));
    //wxCheckBox* cb3 = new wxCheckBox(this, GUI::TIME_PROFILE_CHECKBOX, wxT("Show time profile"),
    //   wxPoint(20, 20));
    //parentGUI->cb2 = new wxCheckBox(this, GUI::SPACE_CALIB_CHECKBOX, wxT("Calibrate playspace"),
    //    wxPoint(20, 20));
    parentGUI->cb3 = new wxCheckBox(this, GUI::MANUAL_CALIB_CHECKBOX, params->language.CAMERA_CALIBRATION_MODE,
        wxPoint(20, 20));
    //parentGUI->cb2->SetValue(false);

    parentGUI->cb4 = new wxCheckBox(this, GUI::MULTICAM_AUTOCALIB_CHECKBOX, params->language.CAMERA_MULTICAM_CALIB,
        wxPoint(20, 20));

    parentGUI->cb5 = new wxCheckBox(this, GUI::LOCK_HEIGHT_CHECKBOX, params->language.CAMERA_LOCK_HEIGHT,
        wxPoint(20, 20));

    cb7->SetValue(true);
    cb8->SetValue(false);

    fgs->Add(btn1);
    fgs->Add(cb1);
    fgs->Add(btn2);
    fgs->Add(cb2);
    fgs->Add(btn3);
    fgs->Add(new wxStaticText(this, -1, wxT("")), 0, wxEXPAND);
    fgs->Add(new wxStaticText(this, -1, params->language.CAMERA_START_STEAMVR), 0, wxEXPAND);
    fgs->Add(new wxStaticText(this, -1, wxT("")), 0, wxEXPAND);
    fgs->Add(btn4);
    fgs->Add(new wxStaticText(this, -1, wxT("")), 0, wxEXPAND);
    fgs->Add(btn5);
    fgs->Add(parentGUI->cb3);
    fgs->Add(cb6);
    fgs->Add(cb7);
    fgs->Add(cb8);
    //fgs->Add(new wxStaticText(this, -1, wxT("")), 0, wxEXPAND);
    //fgs->Add(parentGUI->calibrationModeCheckbox);

    hbox->Add(fgs, 1, wxALL | wxEXPAND, 15);

    //hbox->Add(cb3, 1, wxALL | wxEXPAND, 15);

    parentGUI->posHbox = new wxBoxSizer(wxVERTICAL);
    parentGUI->rotHbox = new wxBoxSizer(wxVERTICAL);

    parentGUI->manualCalibX = new ValueInput(this, "X(cm):", 0);
    parentGUI->manualCalibY = new ValueInput(this, "Y(cm):", 0);
    parentGUI->manualCalibZ = new ValueInput(this, "Z(cm):", 0);
    parentGUI->manualCalibA = new ValueInput(this, wxString::FromUTF8("A(°):"), 0);
    parentGUI->manualCalibB = new ValueInput(this, wxString::FromUTF8("B(°):"), 0);
    parentGUI->manualCalibC = new ValueInput(this, wxString::FromUTF8("C(°):"), 0);

    parentGUI->posHbox->Add(new wxStaticText(this, -1, params->language.CAMERA_CALIBRATION_INSTRUCTION), 0, wxEXPAND);
    parentGUI->posHbox->Add(parentGUI->manualCalibX, 1, wxALL | wxEXPAND, 5);
    parentGUI->posHbox->Add(parentGUI->manualCalibY, 1, wxALL | wxEXPAND, 5);
    parentGUI->posHbox->Add(parentGUI->manualCalibZ, 1, wxALL | wxEXPAND, 5);
    parentGUI->posHbox->Add(parentGUI->manualCalibA, 1, wxALL | wxEXPAND, 5);
    parentGUI->posHbox->Add(parentGUI->manualCalibB, 1, wxALL | wxEXPAND, 5);
    parentGUI->posHbox->Add(parentGUI->manualCalibC, 1, wxALL | wxEXPAND, 5);
    parentGUI->posHbox->Add(parentGUI->cb4);
    parentGUI->posHbox->Add(new wxStaticText(this, -1, wxT("")), 0, wxEXPAND);
    parentGUI->posHbox->Add(parentGUI->cb5);

    hbox->Add(parentGUI->posHbox, 1, wxALL | wxEXPAND, 15);
    //hbox->Add(parentGUI->rotHbox, 1, wxALL | wxEXPAND, 15);

    //hbox2->Show(false);

    this->SetSizer(hbox);

    //parentGUI->posHbox->Show(false);
    //parentGUI->rotHbox->Show(false);
}

ParamsPage::ParamsPage(wxNotebook* parent, Parameters* params, Connection* conn)
    : wxPanel(parent)
    , parameters(params)
    , connection(conn)
    , cameraAddrField1(new wxTextCtrl(this, -1, parameters->cameraAddr1))
    , cameraAddrField2(new wxTextCtrl(this, -1, parameters->cameraAddr2))
    , cameraApiField1(new wxTextCtrl(this, -1, std::to_string(parameters->cameraApiPreference1)))
    , cameraApiField2(new wxTextCtrl(this, -1, std::to_string(parameters->cameraApiPreference2)))
    , trackerNumField(new wxTextCtrl(this, -1, std::to_string(parameters->trackerNum)))
    , markerSizeField(new wxTextCtrl(this, -1, std::to_string(parameters->markerSize * 100)))
    //, prevValuesField(new wxTextCtrl(this, -1, std::to_string(parameters->numOfPrevValues)))
    , smoothingField(new wxTextCtrl(this, -1, std::to_string(parameters->smoothingFactor)))
    , quadDecimateField(new wxTextCtrl(this, -1, std::to_string(parameters->quadDecimate)))
    , searchWindowField(new wxTextCtrl(this, -1, std::to_string(parameters->searchWindow1)))
    // usePredictiveField(new wxCheckBox(this, -1, wxT("")))
    // calibrationTrackerField(new wxTextCtrl(this, -1, std::to_string(parameters->calibrationTracker)))
    , ignoreTracker0Field(new wxCheckBox(this, -1, wxT("")))
    , rotateClField1(new wxCheckBox(this, -1, wxT("")))
    , rotateClField2(new wxCheckBox(this, -1, wxT("")))
    , rotateCounterClField1(new wxCheckBox(this, -1, wxT("")))
    , rotateCounterClField2(new wxCheckBox(this, -1, wxT("")))
    // offsetxField(new wxTextCtrl(this, -1, std::to_string(parameters->calibOffsetX1)))
    // offsetyField(new wxTextCtrl(this, -1, std::to_string(parameters->calibOffsetY1)))
    // offsetzField(new wxTextCtrl(this, -1, std::to_string(parameters->calibOffsetZ1)))
    // circularField(new wxCheckBox(this, -1, wxT("")))
    , camFpsField1(new wxTextCtrl(this, -1, std::to_string(parameters->camFps1)))
    , camFpsField2(new wxTextCtrl(this, -1, std::to_string(parameters->camFps2)))
    , camWidthField1(new wxTextCtrl(this, -1, std::to_string(parameters->camWidth1)))
    , camWidthField2(new wxTextCtrl(this, -1, std::to_string(parameters->camWidth2)))
    , camHeightField1(new wxTextCtrl(this, -1, std::to_string(parameters->camHeight1)))
    , camHeightField2(new wxTextCtrl(this, -1, std::to_string(parameters->camHeight2)))
    , camLatencyField(new wxTextCtrl(this, -1, std::to_string(parameters->camLatency)))
    , cameraSettingsField1(new wxCheckBox(this, -1, wxT("")))
    , cameraSettingsField2(new wxCheckBox(this, -1, wxT("")))
    , settingsParametersField1(new wxCheckBox(this, -1, wxT("")))
    , settingsParametersField2(new wxCheckBox(this, -1, wxT("")))
    , cameraAutoexposureField1(new wxTextCtrl(this, -1, std::to_string(parameters->cameraAutoexposure1)))
    , cameraAutoexposureField2(new wxTextCtrl(this, -1, std::to_string(parameters->cameraAutoexposure2)))
    , cameraExposureField1(new wxTextCtrl(this, -1, std::to_string(parameters->cameraExposure1)))
    , cameraExposureField2(new wxTextCtrl(this, -1, std::to_string(parameters->cameraExposure2)))
    , cameraGainField1(new wxTextCtrl(this, -1, std::to_string(parameters->cameraGain1)))
    , cameraGainField2(new wxTextCtrl(this, -1, std::to_string(parameters->cameraGain2)))
    //, chessboardCalibField(new wxCheckBox(this, -1, wxT("")))
    , trackerCalibCentersField(new wxCheckBox(this, -1, wxT("")))
    , depthSmoothingField(new wxTextCtrl(this, -1, std::to_string(parameters->depthSmoothing)))
    , additionalSmoothingField(new wxTextCtrl(this, -1, std::to_string(parameters->additionalSmoothing)))
{
    //usePredictiveField->SetValue(parameters->usePredictive);
    ignoreTracker0Field->SetValue(parameters->ignoreTracker0);
    rotateClField1->SetValue(parameters->rotateCl1);
    rotateClField2->SetValue(parameters->rotateCl2);
    rotateCounterClField1->SetValue(parameters->rotateCounterCl1);
    rotateCounterClField2->SetValue(parameters->rotateCounterCl2);
    //circularField->SetValue(parameters->circularWindow);
    cameraSettingsField1->SetValue(parameters->cameraSettings1);
    cameraSettingsField2->SetValue(parameters->cameraSettings2);
    //chessboardCalibField->SetValue(parameters->chessboardCalib);
    settingsParametersField1->SetValue(parameters->settingsParameters1);
    settingsParametersField2->SetValue(parameters->settingsParameters2);
    trackerCalibCentersField->SetValue(parameters->trackerCalibCenters);

    wxArrayString markerLibraryValues;
    markerLibraryValues.Add(wxT("ApriltagStandard"));
    markerLibraryValues.Add(wxT("ApriltagCircular"));
    markerLibraryValues.Add(wxT("Aruco4x4"));
    markerLibraryValues.Add(wxT("ApriltagColor"));
    markerLibraryValues.Add(wxT("ApriltagCustom29h10"));

    markerLibraryField = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, markerLibraryValues);
    markerLibraryField->SetSelection(parameters->markerLibrary);

    wxArrayString languageValues;
    languageValues.Add(params->language.LANGUAGE_ENGLISH);
    languageValues.Add(params->language.LANGUAGE_CHINESE);

    languageField = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, languageValues);
    languageField->SetSelection(parameters->languageSelection);

    wxBoxSizer* hbox = new wxBoxSizer(wxVERTICAL);

    wxFlexGridSizer* fgs = new wxFlexGridSizer(4, 10, 10);

    addTextWithTooltip(this, fgs, params->language.PARAMS_LANGUAGE, params->language.PARAMS_LANGUAGE);
    fgs->Add(languageField);

    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, params->language.PARAMS_CAMERA1));
    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));

    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_ID, params->language.PARAMS_CAMERA_TOOLTIP_ID);
    fgs->Add(cameraAddrField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_API, params->language.PARAMS_CAMERA_TOOLTIP_API);
    fgs->Add(cameraApiField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_ROT_CLOCKWISE, params->language.PARAMS_CAMERA_TOOLTIP_ROT_CLOCKWISE);
    fgs->Add(rotateClField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_ROT_CCLOCKWISE, params->language.PARAMS_CAMERA_TOOLTIP_ROT_CCLOCKWISE);
    fgs->Add(rotateCounterClField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_WIDTH, params->language.PARAMS_CAMERA_TOOLTIP_WIDTH);
    fgs->Add(camWidthField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_HEIGHT, params->language.PARAMS_CAMERA_TOOLTIP_HEIGHT);
    fgs->Add(camHeightField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_FPS, params->language.PARAMS_CAMERA_TOOLTIP_FPS);
    fgs->Add(camFpsField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_SETTINGS, params->language.PARAMS_CAMERA_TOOLTIP_SETTINGS);
    fgs->Add(cameraSettingsField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_3_OPTIONS, params->language.PARAMS_CAMERA_TOOLTIP_3_OPTIONS);
    fgs->Add(settingsParametersField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_AUTOEXP, params->language.PARAMS_CAMERA_TOOLTIP_AUTOEXP);
    fgs->Add(cameraAutoexposureField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_EXP, params->language.PARAMS_CAMERA_TOOLTIP_EXP);
    fgs->Add(cameraExposureField1);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_GAIN, params->language.PARAMS_CAMERA_TOOLTIP_GAIN);
    fgs->Add(cameraGainField1);

    fgs->Add(new wxStaticText(this, -1, params->language.PARAMS_CAMERA2));
    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));

    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_ID, params->language.PARAMS_CAMERA_TOOLTIP_ID);
    fgs->Add(cameraAddrField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_API, params->language.PARAMS_CAMERA_TOOLTIP_API);
    fgs->Add(cameraApiField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_ROT_CLOCKWISE, params->language.PARAMS_CAMERA_TOOLTIP_ROT_CLOCKWISE);
    fgs->Add(rotateClField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_ROT_CCLOCKWISE, params->language.PARAMS_CAMERA_TOOLTIP_ROT_CCLOCKWISE);
    fgs->Add(rotateCounterClField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_WIDTH, params->language.PARAMS_CAMERA_TOOLTIP_WIDTH);
    fgs->Add(camWidthField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_HEIGHT, params->language.PARAMS_CAMERA_TOOLTIP_HEIGHT);
    fgs->Add(camHeightField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_FPS, params->language.PARAMS_CAMERA_TOOLTIP_FPS);
    fgs->Add(camFpsField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_SETTINGS, params->language.PARAMS_CAMERA_TOOLTIP_SETTINGS);
    fgs->Add(cameraSettingsField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_3_OPTIONS, params->language.PARAMS_CAMERA_TOOLTIP_3_OPTIONS);
    fgs->Add(settingsParametersField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_AUTOEXP, params->language.PARAMS_CAMERA_TOOLTIP_AUTOEXP);
    fgs->Add(cameraAutoexposureField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_EXP, params->language.PARAMS_CAMERA_TOOLTIP_EXP);
    fgs->Add(cameraExposureField2);
    addTextWithTooltip(this, fgs, params->language.PARAMS_CAMERA_NAME_GAIN, params->language.PARAMS_CAMERA_TOOLTIP_GAIN);
    fgs->Add(cameraGainField2);

    fgs->Add(new wxStaticText(this, -1, params->language.PARAMS_TRACKER));
    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));

    addTextWithTooltip(this, fgs, params->language.PARAMS_TRACKER_NAME_NUM_TRACKERS, params->language.PARAMS_TRACKER_TOOLTIP_NUM_TRACKERS);
    fgs->Add(trackerNumField);
    addTextWithTooltip(this, fgs, params->language.PARAMS_TRACKER_NAME_MARKER_SIZE, params->language.PARAMS_TRACKER_TOOLTIP_MARKER_SIZE);
    fgs->Add(markerSizeField);
    addTextWithTooltip(this, fgs, params->language.PARAMS_TRACKER_NAME_QUAD_DECIMATE, params->language.PARAMS_TRACKER_TOOLTIP_QUAD_DECIMATE);
    fgs->Add(quadDecimateField);
    addTextWithTooltip(this, fgs, params->language.PARAMS_TRACKER_NAME_SEARCH_WINDOW, params->language.PARAMS_TRACKER_TOOLTIP_SEARCH_WINDOW);
    fgs->Add(searchWindowField);
    addTextWithTooltip(this, fgs, params->language.PARAMS_TRACKER_NAME_MARKER_LIBRARY, params->language.PARAMS_TRACKER_TOOLTIP_MARKER_LIBRARY);
    fgs->Add(markerLibraryField);
    addTextWithTooltip(this, fgs, params->language.PARAMS_TRACKER_NAME_USE_CENTERS, params->language.PARAMS_TRACKER_TOOLTIP_USE_CENTERS);
    fgs->Add(trackerCalibCentersField);
    addTextWithTooltip(this, fgs, params->language.PARAMS_TRACKER_NAME_IGNORE_0, params->language.PARAMS_TRACKER_TOOLTIP_IGNORE_0);
    fgs->Add(ignoreTracker0Field);

    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, params->language.PARAMS_SMOOTHING));
    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));

    //addTextWithTooltip(this, fgs, "Number of values for smoothing", "Used to remove pose outliers. Can usually be lowered to 3 to reduce latency.");
    //fgs->Add(prevValuesField);
    addTextWithTooltip(this, fgs, params->language.PARAMS_SMOOTHING_NAME_WINDOW, params->language.PARAMS_SMOOTHING_TOOLTIP_WINDOW);
    fgs->Add(smoothingField);
    addTextWithTooltip(this, fgs, params->language.PARAMS_SMOOTHING_NAME_ADDITIONAL, params->language.PARAMS_SMOOTHING_TOOLTIP_ADDITIONAL);
    fgs->Add(additionalSmoothingField);
    addTextWithTooltip(this, fgs, params->language.PARAMS_SMOOTHING_NAME_DEPTH, params->language.PARAMS_SMOOTHING_TOOLTIP_DEPTH);
    fgs->Add(depthSmoothingField);
    addTextWithTooltip(this, fgs, params->language.PARAMS_SMOOTHING_NAME_CAM_LATENCY, params->language.PARAMS_SMOOTHING_TOOLTIP_CAM_LATENCY);
    fgs->Add(camLatencyField);


    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));

    fgs->Add(new wxStaticText(this, -1, wxT("")));
    fgs->Add(new wxStaticText(this, -1, wxT("")));

    fgs->Add(new wxStaticText(this, -1, params->language.PARAMS_HOVER_HELP));
    wxButton* btn1 = new wxButton(this, SAVE_BUTTON, params->language.PARAMS_SAVE);
    //wxButton* btn2 = new wxButton(this, HELP_BUTTON, "Help");
    Connect(SAVE_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ParamsPage::SaveParams));
    //Connect(HELP_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ParamsPage::ShowHelp));
    fgs->Add(btn1);

    hbox->Add(fgs, 1, wxALL | wxEXPAND, 15);


    this->SetSizer(hbox);
}

void ParamsPage::ShowHelp(wxCommandEvent& event)
{
    wxMessageDialog dial(NULL, wxString::FromUTF8(
        "Short descriptions of main parameters \n\n"
        "Check the github for full tutorial and parameter descriptions!\n\n"
        "Parameters you have to set before starting:\n"
        "- Ip or ID of camera: will be a number 0-10 for USB cameras and \nhttp://'ip - here':8080/video for IP webcam\n"
        "- Number of trackers: set to 3 for full body. 2 will not work in vrchat!\n"
        "- Size of markers: Measure the white square on markers.\n"
        "- Quad decimate: can be 1, 1.5, 2, 3, 4. Higher values will increase FPS, but reduce maximum range of detections\n"
        "- Camera FPS, width, height: Set the fps. Width and height should be fine on 0, but change it in case camera doesn't work correctly.\n\n"
        "Other usefull parameters:\n"
        "- Rotate camera: Self explanatory. Use both for a 180° flip\n"
        "- Number of values for smoothing: Used to remove pose outliers. Can usually be lowered to 3 to reduce latency.\n"
        "- Additional smoothing: 0 to be fast, but very shaky, 1 to barely move the tracker, but smoothly. Experiment to find the sweet spot\n"
        "- Ignore tracker 0: If you want to replace the hip tracker with a vive tracker/owotrack, check this option. Keep number of trackers on 3.\n\n"
        "Experimental:\n"
        "- Camera latency: Increasing this value can help with camera latency. 1 seems to work best.\n"
        "- Use chessboard calibration: Use the old chessboard calibration. It is not recommended, but if you just have a chessboard and cant print a new board yet, you can check this.\n\n"
        "Keep other parameters as default unless you know what you are doing.\n\n"
        "Press OK to close this window."), wxT("Message"), wxOK);
    dial.ShowModal();
}

void ParamsPage::SaveParams(wxCommandEvent& event)
{
    try {
        parameters->cameraAddr1 = cameraAddrField1->GetValue().ToStdString();
        parameters->cameraAddr2 = cameraAddrField2->GetValue().ToStdString();
        parameters->cameraApiPreference1 = std::stoi(cameraApiField1->GetValue().ToStdString());
        parameters->cameraApiPreference2 = std::stoi(cameraApiField2->GetValue().ToStdString());
        parameters->trackerNum = std::stoi(trackerNumField->GetValue().ToStdString());
        parameters->markerSize = std::stod(markerSizeField->GetValue().ToStdString()) / 100;
        parameters->numOfPrevValues = 1; //std::stoi(prevValuesField->GetValue().ToStdString());
        parameters->quadDecimate = std::stod(quadDecimateField->GetValue().ToStdString());
        parameters->searchWindow1 = std::stod(searchWindowField->GetValue().ToStdString());
        //parameters->usePredictive = usePredictiveField->GetValue();
        //parameters->calibrationTracker = std::stoi(calibrationTrackerField->GetValue().ToStdString());
        parameters->ignoreTracker0 = ignoreTracker0Field->GetValue();
        parameters->rotateCl1 = rotateClField1->GetValue();
        parameters->rotateCl2 = rotateClField2->GetValue();
        parameters->rotateCounterCl1 = rotateCounterClField1->GetValue();
        parameters->rotateCounterCl2 = rotateCounterClField2->GetValue();
        //parameters->coloredMarkers = coloredMarkersField->GetValue();
        //parameters->calibOffsetX1 = std::stod(offsetxField->GetValue().ToStdString());
        //parameters->calibOffsetY1 = std::stod(offsetyField->GetValue().ToStdString());
        //parameters->calibOffsetZ1 = std::stod(offsetzField->GetValue().ToStdString());
       // parameters->circularWindow = circularField->GetValue();
        parameters->smoothingFactor = std::stod(smoothingField->GetValue().ToStdString());
        parameters->camFps1 = std::stoi(camFpsField1->GetValue().ToStdString());
        parameters->camFps2 = std::stoi(camFpsField2->GetValue().ToStdString());
        parameters->camWidth1 = std::stoi(camWidthField1->GetValue().ToStdString());
        parameters->camWidth2 = std::stoi(camWidthField2->GetValue().ToStdString());
        parameters->camHeight1 = std::stoi(camHeightField1->GetValue().ToStdString());
        parameters->camHeight2 = std::stoi(camHeightField2->GetValue().ToStdString());
        parameters->camLatency = std::stod(camLatencyField->GetValue().ToStdString());
        parameters->cameraSettings1 = cameraSettingsField1->GetValue();
        parameters->cameraSettings2 = cameraSettingsField2->GetValue();
        parameters->settingsParameters1 = settingsParametersField1->GetValue();
        parameters->settingsParameters2 = settingsParametersField2->GetValue();
        parameters->cameraAutoexposure1 = std::stoi(cameraAutoexposureField1->GetValue().ToStdString());
        parameters->cameraAutoexposure2 = std::stoi(cameraAutoexposureField2->GetValue().ToStdString());
        parameters->cameraExposure1 = std::stoi(cameraExposureField1->GetValue().ToStdString());
        parameters->cameraExposure2 = std::stoi(cameraExposureField2->GetValue().ToStdString());
        parameters->cameraGain1 = std::stoi(cameraGainField1->GetValue().ToStdString());
        parameters->cameraGain2 = std::stoi(cameraGainField2->GetValue().ToStdString());
        parameters->chessboardCalib = false;// chessboardCalibField->GetValue();
        parameters->trackerCalibCenters = trackerCalibCentersField->GetValue();
        parameters->depthSmoothing = std::stod(depthSmoothingField->GetValue().ToStdString());
        parameters->additionalSmoothing = std::stod(additionalSmoothingField->GetValue().ToStdString());
        parameters->markerLibrary = markerLibraryField->GetSelection();
        int prevLanguage = parameters->languageSelection;
        parameters->languageSelection = languageField->GetSelection();
        parameters->Save();

        if (parameters->depthSmoothing > 1)
            parameters->depthSmoothing = 1;
        if (parameters->depthSmoothing < 0)
            parameters->depthSmoothing = 0;

        if (parameters->languageSelection != prevLanguage)
        {
            wxMessageDialog dial(NULL,
                parameters->language.PARAMS_NOTE_LANGUAGECHANGE, wxT("Warning"), wxOK | wxICON_WARNING);
            dial.ShowModal();
        }

        if (parameters->smoothingFactor < 0.2)
        {
            wxMessageDialog dial(NULL,
                parameters->language.PARAMS_NOTE_LOW_SMOOTHING, wxT("Warning"), wxOK | wxICON_WARNING);
            dial.ShowModal();
        }

        if (parameters->quadDecimate != 1 && parameters->quadDecimate != 1.5 && parameters->quadDecimate != 2 && parameters->quadDecimate != 3 && parameters->quadDecimate != 4)
        {
            wxMessageDialog dial(NULL,
                parameters->language.PARAMS_NOTE_QUAD_NONSTANDARD, wxT("Warning"), wxOK | wxICON_WARNING);
            dial.ShowModal();
        }

        if (parameters->cameraSettings1 && parameters->cameraApiPreference1 != 700)
        {
            wxMessageDialog dial(NULL,
                parameters->language.PARAMS_NOTE_NO_DSHOW_CAMSETTINGS, wxT("Warning"), wxOK | wxICON_WARNING);
            dial.ShowModal();
        }

        if (parameters->smoothingFactor <= parameters->camLatency)
        {
            wxMessageDialog dial(NULL,
                parameters->language.PARAMS_NOTE_LATENCY_GREATER_SMOOTHING, wxT("Warning"), wxOK | wxICON_WARNING);
            dial.ShowModal();
        }

        if (parameters->smoothingFactor > 1)
        {
            wxMessageDialog dial(NULL,
                parameters->language.PARAMS_NOTE_HIGH_SMOOTHING, wxT("Warning"), wxOK | wxICON_WARNING);
            dial.ShowModal();
        }

        if (ignoreTracker0Field->GetValue() && std::stoi(trackerNumField->GetValue().ToStdString()) == 2)
        {
            wxMessageDialog dial(NULL,
                parameters->language.PARAMS_NOTE_2TRACKERS_IGNORE0, wxT("Warning"), wxOK | wxICON_WARNING);
            dial.ShowModal();
        }

        wxMessageDialog dial(NULL,
            parameters->language.PARAMS_SAVED_MSG, wxT("Info"), wxOK | wxICON_INFORMATION);
        dial.ShowModal();

        if (connection->status == connection->CONNECTED)
        {
            connection->Send("settings 120 " + std::to_string(parameters->smoothingFactor) + " " + std::to_string(parameters->additionalSmoothing));
        }
        
    }
    catch (std::exception&)
    {
        wxMessageDialog dial(NULL,
            parameters->language.PARAMS_WRONG_VALUES, wxT("Error"), wxOK | wxICON_ERROR);
        dial.ShowModal();
    }
    
}

ValueInput::ValueInput(wxPanel* parent, const wxString& nm, double val)
    : wxPanel(parent)
    , value(val)
    , input(new wxTextCtrl(this, 5, std::to_string(0)))
{
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ValueInput::ButtonPressed));
    Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ValueInput::ButtonPressed));
    Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(ValueInput::MouseScroll));

    hbox->Add(new wxStaticText(this, -1, nm, wxDefaultPosition, wxSize(40, 20)));
    hbox->Add(new wxButton(this, 2, "<<", wxDefaultPosition, wxSize(25, 25)));
    hbox->Add(new wxButton(this, 1, "<", wxDefaultPosition, wxSize(25,25)));
    hbox->Add(input);
    hbox->Add(new wxButton(this, 3, ">", wxDefaultPosition, wxSize(25, 25)));
    hbox->Add(new wxButton(this, 4, ">>", wxDefaultPosition, wxSize(25, 25)));

    this->SetSizer(hbox);
}

void ValueInput::SetValue(double val)
{
    input->ChangeValue(std::to_string(val));
    value = val;
}

void ValueInput::MouseScroll(wxMouseEvent& evt)
{
    if (evt.GetWheelRotation() > 0)
    {
        value += 1;
    }
    else
    {
        value -= 1;
    }
    input->ChangeValue(std::to_string(value));
}

void ValueInput::ButtonPressed(wxCommandEvent &evt)
{
    int id = evt.GetId();
    if (id == 1)
    {
        value -= 1;
    }
    if (id == 2)
    {
        value -= 10;
    }
    if (id == 3)
    {
        value += 1;
    }
    if (id == 4)
    {
        value += 10;
    }
    if (id == 5)
    {
        try {
            value = std::stod(input->GetValue().ToStdString());
        }
        catch (std::exception&)
        {
        }
        return;
    }
    input->ChangeValue(std::to_string(value));
}

// Project headres
#include "MyMainFrame.h"


const float MIN_TEMP = 0, MAX_TEMP = 50;
const int TIME = 10;

int MyMainFrame::m_count = 0;

bool MyMainFrame::ConnectCamera()
{
    try
    {
	bool isMain;
	long adu;
	double eADU;
	double fwc;
	QSICamera::CameraState state;

        std::string serial("");
        //std::string * names = NULL;
        std::string desc("");
        std::string info("");
        std::string modelNumber("");
        cam.get_DriverInfo(info);
	std::cout << "qsiapitest version: " << info << "\n";

        //Discover the connected cameras
        int iNumFound;
        std::string camSerial[QSICamera::MAXCAMERAS];// = "";
        std::string camDesc[QSICamera::MAXCAMERAS];// = "";

        cam.get_AvailableCameras(camSerial, camDesc, iNumFound);

        std::cout << "Available cameras: " << std::endl;
        for (int i = 0; i < iNumFound; i++)
            std::cout << camSerial[i] << ":" << camDesc[i] << "\n";
		
        // Get the serial number of the selected camera in the setup dialog box
        cam.get_SelectCamera(serial);
        if (serial.length() !=0 )
            std::cout << "Saved selected camera serial number is " << serial << "\n";
        cam.put_SelectCamera(serial);

        cam.get_IsMainCamera(&isMain);
        cam.put_IsMainCamera(true);

        // Connect to the selected camera and retrieve camera parameters
        std::cout << "Try to connect to camera...\n";
        cam.put_Connected(true);
        std::cout << "Camera connected. \n";
        cam.get_SerialNumber(serial);
        std::cout << "Serial Number: " + serial + "\n";

        // Get Model Number
        cam.get_ModelNumber(modelNumber);
        std::cout << modelNumber << "\n";

	// This app works only with 6 series
        if (modelNumber.substr(0,1) != "6")
		exit(1);

        // Get the camera state
        // It should be idle at this point
        cam.get_CameraState(&state);
	if (state == QSICamera::CameraError)
	{
            std::cout << "--- Camera is in Error state at the init time. Better to reboot camera. ---" << std::endl;
            exit(1);
	}

        cam.put_SoundEnabled(true);
        cam.put_LEDEnabled(true);

        // Get Camera Description
        cam.get_Description(desc);
        std:: cout << desc << "\n";
	
	bool hasShutter;
	cam.get_HasShutter(&hasShutter);
	if (!hasShutter)
	{
	    std::cout << "No shutter. This app works only with camera having the shutter" << std::endl;
	    exit(1);
	}

	std::cout << "Test shutter. Sometimes it get stuck.\n" << "If the camera beeps, that is the error. Reboot" << std::endl;
	cam.put_ManualShutterMode(true);
	cam.put_ManualShutterOpen(true);
        cam.put_ManualShutterOpen(false);
	cam.put_ManualShutterMode(false);

	cam.put_ReadoutSpeed(QSICamera::FastReadout);
	cam.put_ShutterPriority(QSICamera::ShutterPriorityMechanical);


        long maxX, maxY;
        cam.get_CameraXSize(&maxX);
        cam.get_CameraYSize(&maxY);
	std::cout << "Image size: " << maxX << " x " << maxY << std::endl;
        cam.put_StartX(0);
        cam.put_StartY(0);
        cam.put_NumX(maxX);
        cam.put_NumY(maxY);
        cam.put_BinX(1);
        cam.put_BinY(1);

	// Query various camera parameters
	cam.get_ElectronsPerADU(&eADU);
	std::cout << "Electrons per adu: " << eADU << "\n";
	cam.get_FullWellCapacity(&fwc);
	std::cout << "FWC: " << fwc << "\n";
	cam.get_MaxADU(&adu);
	std::cout << "Max. ADU: " << adu << "\n";
	
	cam.get_MinExposureTime(&m_minExposureTime);
	std::cout << "Min. Exposure Time: " << m_minExposureTime << " sec\n";
	cam.get_MaxExposureTime(&m_maxExposureTime);
	std::cout << "Max. Exposure Time: " << m_maxExposureTime << " sec\n";
	m_exposureTime = 0.03;
	std::cout << "Set Standard Exposure Time: " << m_exposureTime << " sec\n";
    }
    catch (std::runtime_error &err)
    {
        std::string text = err.what();
	std::cout << text << "\n";
	std::string last("");
	cam.get_LastError(last);
	std::cout << last << "\n";
	std::cout << "exiting with errors\n";
	return false;
    }

    return true;
}

bool MyMainFrame::CameraChangeShutterMode(bool isOpen)
{
    cam.put_ManualShutterMode(true);
    cam.put_ManualShutterOpen(isOpen);
    cam.put_ManualShutterMode(false);
    return true;
}

bool MyMainFrame::CameraSetExposureTime(double value)
{
    if (value > m_maxExposureTime)
        value = m_maxExposureTime;

    if (value < m_minExposureTime)
        value = m_minExposureTime;

    m_exposureTime = value;
    return true;
}

bool MyMainFrame::DisconnectCamera()
{
    try
    {
        cam.put_FanMode(QSICamera::fanQuiet);
	CameraChangeShutterMode(false);
        cam.put_Connected(false);
        std::cout << "Camera disconnected. \n";
    }
    catch (std::runtime_error &err)
    {
	std::string text = err.what();
	std::cout << text << "\n";
	std::string last("");
	cam.get_LastError(last);
	std::cout << last << "\n";
	std::cout << "exiting with errors\n";
	return false;
    }

    return true;
}

void MyMainFrame::ChangeConnectMode()
{
    // Slot connected to the Clicked() signal.
    // It will toggle labels "Connect" and "Disconnect",
    // also connect the first visible QSI camera.
    fConnect->SetState(kButtonDown);
    if (m_isConnected && DisconnectCamera()) {
        if (m_isOpened)
	{
	    m_isOpened = kFALSE;
	    fOpen->SetText("&Open");
	}

        fConnect->SetText("&Connect");
        m_isConnected = kFALSE;

        fSpeedo->SetScaleValue(MIN_TEMP);
        fSpeedo->SetOdoValue(0);
    } else if (!m_isConnected && ConnectCamera()) {
        fConnect->SetText("&Disconnect");
        m_isConnected = kTRUE;
    }
    fConnect->SetState(kButtonUp);
    
    if (m_isConnected)
    {
        fOpen->SetState(kButtonUp);

	double minExpTime, maxExpTime;
	cam.get_MinExposureTime(&minExpTime);
	cam.get_MaxExposureTime(&maxExpTime);
	fLExpTimeLimits->SetText(Form("Limits: [%.5f, %.5f] sec", minExpTime, maxExpTime));
	fSetExpTime->SetText(Form("%.5f", m_exposureTime));
	fCBShutter->SetEnabled(kTRUE);
	fCBShutter->Select(1);
	fCBReadOut->SetEnabled(kTRUE);
	fCBReadOut->Select(1);
	fCBGain->SetEnabled(kTRUE);
	fCBGain->Select(1);
    }
    else
    {
	fOpen->SetState(kButtonDisabled);
	fPhoto->SetState(kButtonDisabled);
	fLExpTimeLimits->SetText("");
	fSetExpTime->SetText("");
	fCBShutter->SetEnabled(kFALSE);
	fCBReadOut->SetEnabled(kFALSE);
	fCBGain->SetEnabled(kFALSE);
    }
}

void MyMainFrame::ChangeShutterMode()
{
    // Slot connected to the Clicked() signal.
    // It will toggle labels "Open" and "Close",
    // also open or close shutter
    if (!m_isConnected)
        return;

    fOpen->SetState(kButtonDown);
    if (m_isOpened && CameraChangeShutterMode(false)) {
        fOpen->SetText("&Open");
        m_isOpened = kFALSE;
    } else if (!m_isOpened && CameraChangeShutterMode(true)){
        fOpen->SetText("&Close");
        m_isOpened = kTRUE;
    }
    fOpen->SetState(kButtonUp);

    if (m_isOpened)
        fPhoto->SetState(kButtonUp);
    else
        fPhoto->SetState(kButtonDisabled);
}

void MyMainFrame::ChangeExposureTime()
{
    if (!m_isConnected)
        return;

    const char* text = fSetExpTime->GetText();
    double value = strtod(text, NULL);
    if (value == 0.0)
    {
        std::cout << "Invalid value!" << std::endl;
	value = 0.003;
    }

    CameraSetExposureTime(value);
    fSetExpTime->SetText(Form("%.5f", m_exposureTime));
    std::cout << "Change exposure time " << m_exposureTime << std::endl;
}

void MyMainFrame::ChangeShutterPriority()
{
    int id = fCBShutter->GetSelected();
    if (id == 1)
        cam.put_ShutterPriority(QSICamera::ShutterPriorityMechanical);
    else
        cam.put_ShutterPriority(QSICamera::ShutterPriorityElectronic);

    CameraChangeShutterMode(m_isOpened);
}

void MyMainFrame::ChangeReadoutSpeed()
{
    int id = fCBReadOut->GetSelected();
    if (id == 1)
        cam.put_ReadoutSpeed(QSICamera::FastReadout);
    else
        cam.put_ReadoutSpeed(QSICamera::HighImageQuality);
}

void MyMainFrame::ChangeGain()
{
    int id = fCBGain->GetSelected();
    if (id == 1)
        cam.put_CameraGain(QSICamera::CameraGainHigh);
    else
        cam.put_CameraGain(QSICamera::CameraGainLow);

    double eADU;
    cam.get_ElectronsPerADU(&eADU);
    std::cout << "Set Electrons Per ADU: " << eADU << std::endl;
}

void MyMainFrame::MakePhoto()
{
    fPhoto->SetState(kButtonDisabled);
    CameraMakePhoto();
    fPhoto->SetState(kButtonUp);
    CameraChangeShutterMode(m_isOpened);
}

bool MyMainFrame::CameraMakePhoto()
{
    int x, y, z;

    std::cout << "Starting exposure  with " << m_exposureTime << "s exposure time, Exposure #"<< m_count << " ...\n";
    bool result = cam.StartExposure(m_exposureTime, true);
    if (result != 0) 
    {
        std::cout << "StartExposure error \n";
        std::string last("");
        cam.get_LastError(last);
        std::cout << last << "\n";
        return false;
    }
    std::cout << "Start Exposure Complete.  \nWaiting for Image Ready...\n";

    bool imageReady = false;
    result = cam.get_ImageReady(&imageReady);
    while(!imageReady)
    {
        //usleep(TIME);
        result = cam.get_ImageReady(&imageReady);
        if (result != 0) 
        {
            std::cout << "get_ImageReady error \n";
            std::string last("");
            cam.get_LastError(last);
            std::cout << last << "\n";
	    return false;
	}
    }
    std::cout << "Image Ready...\n";

    result = cam.get_ImageArraySize(x, y, z);
    if (result != 0) 
    {
        std::cout << "get_ImageArraySize error \n";
        std::string last("");
        cam.get_LastError(last);
        std::cout << last << "\n";
        return false;
    }
    std::cout << "Image Size " << x << " x " << y << " " << x * y << " Pixels...\n";

    struct timespec start, finish;
    clock_gettime(CLOCK_REALTIME, &start);

    unsigned short* image = new unsigned short[x * y];
    // Retrieve the pending image from the camera
    result = cam.get_ImageArray(image);
    if (result != 0) 
    {
        std::cout << "get_ImageArray error \n";
        std::string last("");
        cam.get_LastError(last);
        std::cout << last << "\n";
        delete [] image;
        return false;
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    printf("Read time %.9f sec\n", (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) * 1E-9);
    
    QueueJob(image, x, y);

    return true;
}

void MyMainFrame::QueueJob(unsigned short* image, int cols, int rows)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push({image, cols, rows});
    }
    launchCondition.notify_one();
}

void MyMainFrame::ThreadLoop()
{
    while (true)
    {
        std::tuple<unsigned short*, int, int> jPars;
	{
            std::unique_lock<std::mutex> lock(queue_mutex);
	    launchCondition.wait(lock, [this](){
			    	return !jobs.empty();});

	    jPars = jobs.front();
	    jobs.pop();
	}
	SaveImage(std::get<0>(jPars), std::get<1>(jPars), std::get<2>(jPars));
    }
}

bool MyMainFrame::SaveImage(unsigned short* image, int cols, int rows)
{
    TString filename = Form("pics/photo_%d.root", m_count);
    std::cout << "Wrtie objects to " << filename << std::endl;

    struct timespec start, finish;
    clock_gettime(CLOCK_REALTIME, &start);

    TFile file(filename, "RECREATE");
    TH2I hist2d ("hist2d", "hist2d", cols, 0, cols, rows, 0, rows);
    for (int j = 0; j < rows; j++)
        for (int i = 0; i < cols; i++)
            hist2d.SetBinContent(i, j, image[(j * cols) + i]);

    TParameter<double> expTimeT("exposureTime", m_exposureTime);
    expTimeT.Write();

    TDatime time;
    TNamed dataT("data", time.AsString());
    dataT.Write();

    int id = fCBShutter->GetSelected();
    TString shutterPriority;
    if (id == 1)
        shutterPriority = "ShutterPriorityMechanical";
    else
        shutterPriority = "ShutterPriorityElectronic";
    TNamed shutterT("shutterPriority", shutterPriority.Data());
    shutterT.Write();

    id = fCBReadOut->GetSelected();
    TString readout;
    if (id == 1)
        readout = "FastReadout";
    else
        readout = "HighImageQuality";
    TNamed readT("readoutSpeed", readout.Data());
    readT.Write();
    
    id = fCBGain->GetSelected();
    TString gain;
    if (id == 1)
        gain = "HighGain";
    else
        gain = "LowGain";
    TNamed gainT("gainType", gain.Data());
    gainT.Write();

    double eADU;
    cam.get_ElectronsPerADU(&eADU);
    TParameter<double> eADUT("ePerADU", eADU);
    eADUT.Write();

    TParameter<double> xSizeT("xSize", cols);
    xSizeT.Write();
    TParameter<double> ySizeT("ySize", rows);
    ySizeT.Write();

    short unsigned int mean;
    cam.get_LastOverscanMean(&mean);
    TParameter<double> meanT("mean", mean);
    meanT.Write();

    hist2d.Write();
    file.Close();
    hist2d.Delete();

    m_count++;

    delete [] image;
    std::cout << "Finish saving" << std::endl;
    clock_gettime(CLOCK_REALTIME, &finish);
    printf("Converting time %.9f sec\n", (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) * 1E-9);

    return true;
}

void MyMainFrame::Update()
{
    if (!m_isConnected)
	    return;

    double temperature;
    QSICamera::FanMode fanMode;
    try
    {
        cam.get_CCDTemperature(&temperature);
        cam.get_FanMode(fanMode);
    }
    catch (std::runtime_error &err)
    {
	std::cout << "Can't get the temperature or fan mode" << std::endl;
        std::string text = err.what();
	std::cout << text << "\n";
	std::string last("");
	cam.get_LastError(last);
	std::cout << last << "\n";
	std::cout << "exiting with errors\n";
	return;
    }

    fSpeedo->SetScaleValue(temperature);

    float fanValue;
    switch (fanMode){
	    case QSICamera::fanOff: fanValue = 0.; break;
	    case QSICamera::fanQuiet: fanValue = 50.; break;
	    case QSICamera::fanFull: fanValue = 100.; break;
	    default: fanValue = 100;
    }
    fSpeedo->SetOdoValue(fanValue);
}

void MyMainFrame::ChangeFanMode()
{
    if (!m_isConnected)
        return;

    QSICamera::FanMode fanMode;
    try
    {
        cam.get_FanMode(fanMode);
        switch (fanMode){
	    case QSICamera::fanOff: fanMode = QSICamera::fanQuiet; break;
	    case QSICamera::fanQuiet: fanMode = QSICamera::fanFull; break;
	    case QSICamera::fanFull: fanMode = QSICamera::fanOff; break;
        }
	cam.put_FanMode(fanMode);
	CameraChangeShutterMode(m_isOpened);
    }
    catch (std::runtime_error &err)
    {
	std::cout << "Can't change fan mode" << std::endl;
        std::string text = err.what();
	std::cout << text << "\n";
	std::string last("");
	cam.get_LastError(last);
	std::cout << last << "\n";
	std::cout << "exiting with errors\n";
	return;
    }
}

MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) :
  TGMainFrame(p, w, h)
{
    cam.put_UseStructuredExceptions(true);

    // Create a horizontal frame containing buttons
    fCframe = new TGCompositeFrame(this, 300, 50, kHorizontalFrame);//|kFixedWidth);

    fConnect = new TGTextButton(fCframe, "&Connect");
    fConnect->Connect("Clicked()", "MyMainFrame", this, "ChangeConnectMode()");
    fCframe->AddFrame(fConnect, new TGLayoutHints(kLHintsTop | kLHintsExpandX,
                                               3, 2, 2, 2));
    fConnect->SetToolTipText("Click to (Connect/Disconnect) the camera");
    m_isConnected = kFALSE;

    fOpen = new TGTextButton(fCframe, "&Open");
    fOpen->Connect("Clicked()", "MyMainFrame", this, "ChangeShutterMode()");
    fOpen->SetToolTipText("Click to (Open/Close) the camera shutter");
    fCframe->AddFrame(fOpen, new TGLayoutHints(kLHintsTop | kLHintsExpandX,
                                               3, 2, 2, 2));
    fOpen->SetState(kButtonDisabled);
    m_isOpened = kFALSE;

    AddFrame(fCframe, new TGLayoutHints(kLHintsCenterX | kLHintsTop | kLHintsExpandX, 2, 2, 5, 1));

    fPhoto = new TGTextButton(this, "&Photo");
    fPhoto->Connect("Clicked()", "MyMainFrame", this, "MakePhoto()");
    AddFrame(fPhoto, new TGLayoutHints(kLHintsTop | kLHintsExpandX ,5,5,2,2));
    fPhoto->SetState(kButtonDisabled);


    // Exposure time
    fCframeExposure = new TGCompositeFrame(this, 300, 50, kHorizontalFrame | kRaisedFrame | kDoubleBorder);
    fLExpTime = new TGLabel(fCframeExposure, "Exposure Time:");
    fCframeExposure->AddFrame(fLExpTime, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 3, 3, 3, 3));

    fSetExpTime = new TGTextEntry(fCframeExposure);
    fSetExpTime->SetAlignment(kTextCenterX);
    fCframeExposure->AddFrame(fSetExpTime, new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX, 3, 3, 3, 3));
    fSetExpTime->Connect("ReturnPressed()", "MyMainFrame", this, "ChangeExposureTime()");

    fLExpTimeLimits = new TGLabel(fCframeExposure, "");
    fCframeExposure->AddFrame(fLExpTimeLimits, new TGLayoutHints(kLHintsTop | kLHintsRight | kLHintsExpandX, 3, 3, 3, 3));

    AddFrame(fCframeExposure, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 3, 3, 3));

    // Shutter priority
    fCframeShutter = new TGCompositeFrame(this, 300, 50, kHorizontalFrame | kRaisedFrame | kDoubleBorder);
    fLShutterPriority = new TGLabel(fCframeShutter, "ShutterPriority:");
    fCframeShutter->AddFrame(fLShutterPriority, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 3, 3, 3, 3));

    fCBShutter = new TGComboBox(fCframeShutter); 
    fCBShutter->Resize(1, 20);
    fCBShutter->AddEntry("MechanicalPriority", 1);
    fCBShutter->AddEntry("ElectronicPriority", 2);
    fCframeShutter->AddFrame(fCBShutter, new TGLayoutHints(kLHintsRight | kLHintsExpandX, 3, 3, 3, 3));
    fCBShutter->Connect("Changed()", "MyMainFrame", this, "ChangeShutterPriority()");
    fCBShutter->SetEnabled(kFALSE);

    AddFrame(fCframeShutter, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 3, 3, 3));
    
    // Readout speed
    fCframeReadOut = new TGCompositeFrame(this, 300, 50, kHorizontalFrame | kRaisedFrame | kDoubleBorder);
    fLReadOut = new TGLabel(fCframeReadOut, "Readout Speed:");
    fCframeReadOut->AddFrame(fLReadOut, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 3, 3, 3, 3));

    fCBReadOut = new TGComboBox(fCframeReadOut); 
    fCBReadOut->Resize(1, 20);
    fCBReadOut->AddEntry("FastReadout", 1);
    fCBReadOut->AddEntry("HighImageQuality", 2);
    fCframeReadOut->AddFrame(fCBReadOut, new TGLayoutHints(kLHintsRight | kLHintsExpandX, 3, 3, 3, 3));
    fCBReadOut->Connect("Changed()", "MyMainFrame", this, "ChangeReadoutSpeed()");
    fCBReadOut->SetEnabled(kFALSE);

    AddFrame(fCframeReadOut, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 3, 3, 3));
    
    // Gain
    fCframeGain = new TGCompositeFrame(this, 300, 50, kHorizontalFrame | kRaisedFrame | kDoubleBorder);
    fLGain = new TGLabel(fCframeGain, "Camera Gain:");
    fCframeGain->AddFrame(fLGain, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 3, 3, 3, 3));

    fCBGain = new TGComboBox(fCframeGain); 
    fCBGain->Resize(1, 20);
    fCBGain->AddEntry("High 0.262e-/ADU", 1);
    fCBGain->AddEntry("Low 0.586-/ADU", 2);
    fCBGain->AddEntry("Auto", 2);
    fCframeGain->AddFrame(fCBGain, new TGLayoutHints(kLHintsRight | kLHintsExpandX, 3, 3, 3, 3));
    fCBGain->Connect("Changed()", "MyMainFrame", this, "ChangeGain()");
    fCBGain->SetEnabled(kFALSE);

    AddFrame(fCframeGain, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 3, 3, 3));

    // Pixel size X
    // Pixel size Y

    fExit = new TGTextButton(this, "&Exit ","gApplication->Terminate(0)");
    AddFrame(fExit, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 3, 3, 3));

    fSpeedo = new TGSpeedo(this, 0, 100, "Temp", "C");
    fSpeedo->Connect("LedClicked()", "MyMainFrame", this, "ChangeFanMode()");
    AddFrame(fSpeedo, new TGLayoutHints(kLHintsTop | kLHintsCenterX, 3, 3, 3, 3));
    fSpeedo->SetDisplayText("Fan", "%");
    fSpeedo->SetMinMaxScale(MIN_TEMP, MAX_TEMP);

    fTimer = new TTimer(100);
    fTimer->Connect("Timeout()", "MyMainFrame", this, "Update()");

    SetWindowName("Camera control");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
    fTimer->TurnOn();

    this->Resize(900, 500);
    Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");

    supportWorker = std::thread(&MyMainFrame::ThreadLoop, this);
}

MyMainFrame::~MyMainFrame()
{
    // Clean up all widgets, frames and layouthints that were used
    fCframe->Cleanup();
    Cleanup();
    supportWorker.join();
}

void runFrame()
{
    // Popup the GUI...
    new MyMainFrame(gClient->GetRoot(), 500, 500);
}


int main(int argc, char **argv)
{
    TApplication app("app", &argc, argv);
    runFrame();
    app.Run();
    return 0;
}

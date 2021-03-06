/******************************************************************************/
/* This file has been generated by the uGFX-Studio                            */
/*                                                                            */
/* http://ugfx.org                                                            */
/******************************************************************************/

#include "colors.h"
#include "widgetstyles.h"
#include "gui.h"

// GListeners
GListener glistener;

// GHandles
GHandle ghContainerPage0;
GHandle ghSlider1;
GHandle ghButton1;
GHandle ghCheckbox1;
GHandle ghRadiobutton1;
GHandle ghRadiobutton2;

// Fonts
font_t dejavu_sans_16;

static void createPagePage0(void)
{
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);


	// create container widget: ghContainerPage0
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 480;
	wi.g.height = 272;
	wi.g.parent = 0;
	wi.text = "Container";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainerPage0 = gwinContainerCreate(0, &wi, 0);

	// create button widget: ghSlider1
	wi.g.show = TRUE;
	wi.g.x = 30;
	wi.g.y = 20;
	wi.g.width = 200;
	wi.g.height = 20;
	wi.g.parent = ghContainerPage0;
	wi.text = "Slider1";
	wi.customDraw = gwinSliderDraw_Std;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghSlider1 = gwinSliderCreate(0, &wi);
	gwinSliderSetRange(ghSlider1, 0, 100);
	gwinSliderSetPosition(ghSlider1, 35);
	gwinSetColor(ghSlider1, black_studio);
	gwinSetBgColor(ghSlider1, white_studio);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 290;
	wi.g.y = 20;
	wi.g.width = 120;
	wi.g.height = 20;
	wi.g.parent = ghContainerPage0;
	wi.text = "Button1";
	wi.customDraw = gwinButtonDraw_Normal;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghButton1 = gwinButtonCreate(0, &wi);

	// create checkbox widget: ghCheckbox1
	wi.g.show = TRUE;
	wi.g.x = 30;
	wi.g.y = 110;
	wi.g.width = 120;
	wi.g.height = 20;
	wi.g.parent = ghContainerPage0;
	wi.text = "Checkbox1";
	wi.customDraw = gwinCheckboxDraw_CheckOnLeft;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghCheckbox1 = gwinCheckboxCreate(0, &wi);
	gwinCheckboxCheck(ghCheckbox1, FALSE);

	// create radiobutton widget: ghRadiobutton1
	wi.g.show = TRUE;
	wi.g.x = 300;
	wi.g.y = 110;
	wi.g.width = 120;
	wi.g.height = 20;
	wi.g.parent = ghContainerPage0;
	wi.text = "RadioButton1";
	wi.customDraw = gwinRadioDraw_Radio;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghRadiobutton1 = gwinRadioCreate(0, &wi, 0);
	gwinSetColor(ghRadiobutton1, black_studio);
	gwinSetBgColor(ghRadiobutton1, white_studio);
	gwinRadioPress(ghRadiobutton1);

	// create radiobutton widget: ghRadiobutton2
	wi.g.show = TRUE;
	wi.g.x = 300;
	wi.g.y = 150;
	wi.g.width = 120;
	wi.g.height = 20;
	wi.g.parent = ghContainerPage0;
	wi.text = "RadioButton2";
	wi.customDraw = gwinRadioDraw_Radio;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghRadiobutton2 = gwinRadioCreate(0, &wi, 0);
	gwinSetColor(ghRadiobutton2, black_studio);
	gwinSetBgColor(ghRadiobutton2, white_studio);
}

void guiShowPage(unsigned pageIndex)
{
	// Hide all pages
	gwinHide(ghContainerPage0);

	// Show page selected page
	switch (pageIndex) {
	case 0:
		gwinShow(ghContainerPage0);
		break;

	default:
		break;
	}
}

void guiCreate(void)
{
	GWidgetInit wi;

	// Prepare fonts
	dejavu_sans_16 = gdispOpenFont("DejaVuSans16");

	// Prepare images

	// GWIN settings
	gwinWidgetClearInit(&wi);
	gwinSetDefaultFont(dejavu_sans_16);
	gwinSetDefaultStyle(&white, FALSE);
	gwinSetDefaultColor(black_studio);
	gwinSetDefaultBgColor(white_studio);

	// Create all the display pages
	createPagePage0();

	// Select the default display page
	guiShowPage(0);

}

void guiEventLoop(void)
{
	GEvent* pe;

	while (1) {
		// Get an event
		pe = geventEventWait(&glistener, TIME_INFINITE);
		switch (pe->type) {
		}

	}
}


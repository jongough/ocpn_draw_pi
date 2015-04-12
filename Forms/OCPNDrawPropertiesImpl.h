#ifndef __OCPNDrawPropertiesImpl__
#define __OCPNDrawPropertiesImpl__

/**
@file
Subclass of OCPNDrawPropertiesDialog, which is generated by wxFormBuilder.
*/

#include "OCPNDrawPropertiesDialog.h"

/** Implementing OCPNDrawPropertiesDialog */
class OCPNDrawPropertiesImpl : public OCPNDrawPropertiesDialog
{
protected:
	// Handlers for OCPNDrawPropertiesDialog events.
	void OnDrawPropertiesOKClick( wxCommandEvent& event );
	void OnDrawPropertiesCancelClick( wxCommandEvent& event );
	void OnDrawPropertiesApplyClick( wxCommandEvent& event );
	
public:
	/** Constructor */
	OCPNDrawPropertiesImpl( wxWindow* parent );
};

#endif // __OCPNDrawPropertiesImpl__

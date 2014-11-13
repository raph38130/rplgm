// This is the main project file for VC++ application project
// generated using an Application Wizard.

#include "stdafx.h"
#using <System.Xml.dll>
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Serialization;
using namespace System;


public ref class Demo
{	
public:
	String ^data1;
	int	data2;
	Demo(){}
	Demo(String ^s, int n)
	{
		data1=s;
		data2=n;
	}
};
int _tmain()
{
    Console::WriteLine(L"Serializing...(new style)");
	StreamWriter^ wr= gcnew StreamWriter("demo.xml");

	array<Demo^>^ d={gcnew Demo(L"1",10),gcnew Demo(L"2",20),gcnew Demo(L"3",30)};
	
	XmlSerializer(d->GetType()).Serialize(wr,d);

	wr->Close();
    return 0;
}

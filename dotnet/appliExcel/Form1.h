#pragma once


namespace AppliExcel {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
		
	public:	Microsoft::Office::Interop::Excel::Application^ xap;
			Microsoft::Office::Interop::Excel::Workbook  ^wb;
			Microsoft::Office::Interop::Excel::Worksheet ^ws;
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			xap = gcnew Microsoft::Office::Interop::Excel::Application();
			wb = xap->Workbooks->Add(Microsoft::Office::Interop::Excel::XlWBATemplate::xlWBATWorksheet);

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;

	private: System::Windows::Forms::TextBox^  textBox1;




	private: System::ComponentModel::IContainer^  components;

	protected: 




	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(95, 92);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"import";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(277, 88);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(225, 20);
			this->textBox1->TabIndex = 2;
			this->textBox1->Text = L"c:\\demo.xls";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(657, 528);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {

			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				
				 
				xap->Visible=true;
				
				
			    xap->Workbooks->Open("c:/demo.xls",0, false, 5, nullptr, nullptr, false, Microsoft::Office::Interop::Excel::XlPlatform::xlWindows, nullptr,true, false, 0, true, false, false);
				ws = (Microsoft::Office::Interop::Excel::Worksheet ^)wb->Worksheets[1];	
				
				 
			 }
	};
}


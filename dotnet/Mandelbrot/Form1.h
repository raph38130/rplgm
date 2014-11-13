#pragma once

namespace Mandelbrojt {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//zone intéressante à observer pour la fractale
			/* fractale 0*/
			drawrec=false;
			done=false;
			X1= 1.0;	Y1= 1;
			X2=-2.0;	Y2=-1;

			/* fractale 1
			X1=-1.55;	Y1= 0.1;
			X2=-1.85;	Y2=-0.1;
			*/
			/* fractale 2
			X1=-1.76;	Y1= 0.01;
			X2=-1.79;	Y2=-0.01;*/
		}

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisées.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	private: double X1,Y1,X2,Y2;
	private: Bitmap ^bm;
	private: bool drawrec,done;
	private: int x1,y1,x2,y2;

	private:
		/// <summary>
		/// Variable nécessaire au concepteur.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette méthode avec l'éditeur de code.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(656, 473);
			this->Name = L"Form1";
			this->Text = L"Fractale de Mandelbrot v0.0";
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::Form1_Paint);
			this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseUp);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseMove);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseDown);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: bool converge(double x, double y) {
				 const int count=100;
				 double aux;
				 //return true if M(x,y) is in mandelbrot fractal ?
				 //evaluation of the N first terms of Zn
				 double an=0,bn=0;
				 for (int n=0 ; n < count ; n++) {
					 //real part
					 aux=an;
					 an = an*an - bn*bn + x;
					 //imaginary part
					 bn = 2*aux*bn + y;
					 //if (Math::Sqrt(an*an+bn*bn) > 2) return false;
					 if ((an*an+bn*bn) > 4) return false;
				 }
				 return true;
			 }

	private: System::Void Form1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 Pen ^p1=gcnew Pen(Color::Red);
				 
				 e->Graphics->DrawImage(bm,0,0,bm->Width,bm->Height);
				 if (drawrec) {
					 //1
					 if (x2>x1 && y2>y1)
						 e->Graphics->DrawRectangle(p1,x1,y1,x2-x1,y2-y1);
					 //2
					 if (x2>x1 && y2<y1)
						 e->Graphics->DrawRectangle(p1,x1,y2,x2-x1,y1-y2);
					 //3
					 if (x2<x1 && y2<y1)
						 e->Graphics->DrawRectangle(p1,x2,y2,x1-x2,y1-y2);
					 //4
					 if (x2<x1 && y2>y1)
						 e->Graphics->DrawRectangle(p1,x2,y1,x1-x2,y2-y1);
				 }
			 }

			 private:void drawbitmap() {
				 double X,Y;
				 bm=gcnew Bitmap(Width,Height);
				 //x, y parcourt le bitmap
				 for (int x=0 ; x < Width ; x++)
					 for (int y=0 ; y < Height ; y++) {
						 //changement de repère
						 X = (double)x / Width  * (X1 - X2) + X2;
						 Y = (double)y / Height * (Y2 - Y1) + Y1;
						 if (converge(X,Y))
							 bm->SetPixel(x,y,Color::Red);
					 }
					 }

	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 drawbitmap();
			 }
	private: System::Void Form1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 drawrec=true;
				 x1=e->X;
				 y1=e->Y;
			 }
	private: System::Void Form1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if (drawrec) {
					 x2=e->X;
					 y2=e->Y;
					 Invalidate();
				 }
			 }
	private: System::Void Form1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) { 
			 int x3,y3;
			 double oX1,oX2,oY1,oY2;
			 drawrec=false;
			 x3=e->X;
			 y3=e->Y;
			 oX1=X1;oX2=X2;oY1=Y1;oY2=Y2;

			 X1=(double)x3/Width  * (oX1-oX2) + oX2;
			 X2=(double)x1/Width  * (oX1-oX2) + oX2;
			 Y1=(double)y1/Height * (oY2-oY1) + oY1;
			 Y2=(double)y3/Height * (oY2-oY1) + oY1;

			 //textBox1->Text="X1="+X1+" Y1="+Y1+" X2="+X2+" Y2="+Y2;
			 drawbitmap();
			 Invalidate();
			 }
			};
	}


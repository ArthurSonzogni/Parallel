#ifndef VECTEUR_U68QPGMC
#define VECTEUR_U68QPGMC

	class Vecteur
	{
		public:
			double x,y;
			Vecteur(double X, double Y);
			double length();
	};
	Vecteur operator+(Vecteur p1, Vecteur p2);
	Vecteur operator-(Vecteur p1, Vecteur p2);
	Vecteur operator*(double a, Vecteur p);
	double operator*(Vecteur p1, Vecteur p2);
	double operator^(Vecteur p1, Vecteur p2);
	Vecteur normalize(Vecteur p);
	
#endif /* end of include guard: VECTEUR_U68QPGMC */

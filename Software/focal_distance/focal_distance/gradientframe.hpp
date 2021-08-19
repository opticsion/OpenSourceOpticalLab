class GradientFrame{
	private:
		int number;
	public:
		GradientFrame();
		void gradient_relative(int value,int* rgb,int max);
		void gradient_absolute(int value,int* rgb);
};

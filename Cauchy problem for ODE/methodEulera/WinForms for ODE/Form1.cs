using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ZedGraph;

namespace LB16
{
    public partial class Form1 : Form
    {
        int tickStart = 0;
        public List<double> X = new List<double>();
        public List<double> Y = new List<double>();
        public double[] copyValue = new double[5];//для того чтобы заново не считать график

        public bool flag = false; //если был построен один график то все методы разом нельзя
        int[] active = new int[5] { 0,0,0,0,0 }; // был ли уже расчет в методе или нет
        bool[] activeDraw = new bool[5] { false, false, false, false, false }; //бил ли нарисован уже метод или нет (для все методы разом)

        public Form1()
        {   
            InitializeComponent();    
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            // Очистим список кривых на тот случай, если до этого сигналы уже были нарисованы
            myPane.CurveList.Clear();

            myPane.Title.Text = "Методы решения ОДУ: 3sin(2y)+x";
            myPane.XAxis.Title.Text = " X ";
            myPane.YAxis.Title.Text = " Y ";

            //точки
            RollingPointPairList list = new RollingPointPairList(1200);
            RollingPointPairList list2 = new RollingPointPairList(1200);
            RollingPointPairList list3 = new RollingPointPairList(1200);
            RollingPointPairList list4 = new RollingPointPairList(1200);
            RollingPointPairList list5= new RollingPointPairList(1200);
            // Создадим кривую с названием "Sinc",
            // которая будет рисоваться голубым цветом (Color.Blue),
            // Опорные точки выделяться не будут (SymbolType.None)
            LineItem curve1 = myPane.AddCurve("Эйлер", list, Color.Blue,SymbolType.Square);
            LineItem curve2 = myPane.AddCurve("Рунге-Кутты 2 порядка", list2, Color.Green, SymbolType.Diamond);
            LineItem curve3 = myPane.AddCurve("Рунге-Кутты 4 порядка", list3, Color.Brown, SymbolType.Circle);
            LineItem curve4 = myPane.AddCurve("Адамс-Баншорт 4 порядка", list4, Color.Red, SymbolType.Triangle);
            LineItem curve5 = myPane.AddCurve("Адамс-Мультон 2 порядка", list5, Color.Pink, SymbolType.Plus);


            timer1.Interval = 150;

            // Вызываем метод AxisChange(), чтобы обновить данные об осях.
           // В противном случае на рисунке будет показана только часть графика,
           // которая умещается в интервалы по осям, установленные по умолчанию
            zedGraphControl1.AxisChange();
            tickStart = Environment.TickCount;

        }
       
        public void draw(double setpoint, double setpoint2)
        {
           
            if (zedGraphControl1.GraphPane.CurveList.Count <=0)
            {
                throw new NullReferenceException("Обращение к пустому списку");
                
            }
            if(checkBox1.Checked==true || (allMethod.Checked==true && active[0]==1 && activeDraw[0]==false))
            {
                inputPointList(setpoint, setpoint2, 0);
            }
            if(checkBox2.Checked==true || (allMethod.Checked == true && active[1] == 1 && activeDraw[1] == false))
            {
                inputPointList(setpoint, setpoint2,1);
            }
            if (checkBox3.Checked == true || (allMethod.Checked == true && active[2] == 1 && activeDraw[2] == false))
            {
                inputPointList(setpoint, setpoint2,2);
            }
            if (checkBox4.Checked == true || (allMethod.Checked == true && active[3] == 1 && activeDraw[3] == false))
            {

                inputPointList(setpoint, setpoint2,3);
            }
            if (checkBox5.Checked == true || (allMethod.Checked == true && active[4] == 1 && activeDraw[4] == false))
            {
   
                inputPointList(setpoint, setpoint2,4);
            }

            //  double time = (Environment.TickCount - tickStart) / 1000.0;
            // list1.Add(time, setpoint);


            Scale xscale = zedGraphControl1.GraphPane.XAxis.Scale;
           // if(time > xscale.Max - xscale.MajorStep)
           // {    }
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
        }

        private void inputPointList(double setpoint, double setpoint2, int method)
        {
            LineItem curver1 = null;
            if (method == 0)
            {
                curver1 = zedGraphControl1.GraphPane.CurveList[0] as LineItem;
            }
            if (method == 1)
            {
                curver1 = zedGraphControl1.GraphPane.CurveList[1] as LineItem;
            }
            if (method == 2)
            {
                curver1 = zedGraphControl1.GraphPane.CurveList[2] as LineItem;
            }
            if (method == 3)
            {
                curver1 = zedGraphControl1.GraphPane.CurveList[3] as LineItem;
            }
            if (method == 4)
            {
                curver1 = zedGraphControl1.GraphPane.CurveList[4] as LineItem;
            }
            if (curver1 == null)
            {
                return;//TODO: Исключение
            }
            
            IPointListEdit list1 = curver1.Points as IPointListEdit;
     
            if (list1 == null)
            {
                throw new NullReferenceException("Обращение к пустому списку!");
               // return;//TODO: Исключение
            }
            list1.Add(setpoint, setpoint2);
  
        }

        private void Button1_Click(object sender, EventArgs e)
        {
     
            X.Clear();
            Y.Clear();

            if (NoChoseMethod()==true)
            {
                MessageBox.Show("Ни один метод не выбран!");
                return;
            }else
                if(allMethod.Checked==false)
                    calcucaltion_DQ();

            if(allMethod.Checked == true)
                timer1.Enabled = true;
            if(X.Count!=0 && Y.Count != 0)
            {
                zedGraphControl1.GraphPane.GraphObjList.Clear();
                zedGraphControl1.AxisChange();
                zedGraphControl1.Invalidate();
                timer1.Enabled = true;
            }
              
        }
        private bool NoChoseMethod()
        {
            return checkBox1.Checked == false && checkBox2.Checked == false && checkBox3.Checked == false && checkBox4.Checked==false && checkBox5.Checked==false && allMethod.Checked==false;
        }

        private void Timer1_Tick(object sender, EventArgs e)
        {
            // timer1.Interval = trackBar2.Value;

            if(allMethod.Checked==false)
            {
                allMethod.Enabled = false;
                flag = true;
                inputOnScheme();
                dottedLine();
                timer1.Enabled = false;
            }else
            {
                X.Clear();
                Y.Clear();
                calcucaltion_DQ();
               
                if(activeDraw[0]==false)
                {
                    inputOnScheme();
                    activeDraw[0] = true;
                    return;
                }
                if (activeDraw[1] == false)
                {
                    inputOnScheme();
                    activeDraw[1] = true;
                    return;
                }
                if (activeDraw[2] == false)
                {
                    inputOnScheme();
                    activeDraw[2] = true;
                    return;
                }
                if (activeDraw[3] == false)
                {
                    inputOnScheme();
                    activeDraw[3] = true;
                    return;
                }
                if (activeDraw[4] == false)
                {
                    inputOnScheme();
                    activeDraw[4] = true;
                    dottedLine();
                    return;
                }
                
                if (active[4]==1)
                {
                    for (int i = 0; i < active.Length; i++)
                    {
                        active[i] = 0;
                        activeDraw[i] = false;
                    }
                       
                    timer1.Enabled = false;
                }
                   
            }
            
          
            
        }
        private  void inputOnScheme()
        {
            for (int i = 0; i < X.Count; i++)
            {
                draw(X[i], Y[i]);
                
            }
        }
        /// <summary>
        /// пунктирные линии
        /// </summary>
        private void dottedLine()
        {
            if(X!=null && Y!=null)
            {
                // !!!
                // Линию рисуем после обновления осей с помощью AxisChange (),
                // так как мы будем использовать значения
                // Нарисуем горизонтальную пунктирную линию от левого края до правого на уровне y = 0.5
                double minx = 0, maxx = 0;
                for(int i=0;i<Y.Count;i++)
                {
                    if(Y[i]==Y.Min())
                    {
                        minx = X[i];
                        break;
                    }
                }
                for(int j=0;j<Y.Count;j++)
                {
                    if(Y[j]==Y.Max())
                    {
                        maxx = X[j];
                        break;
                    }
                }
                LineObj Minline = new LineObj(minx, Y.Min(), minx, zedGraphControl1.GraphPane.YAxis.Scale.Min);
                LineObj Maxline2 = new LineObj(maxx, Y.Max(), maxx, zedGraphControl1.GraphPane.YAxis.Scale.Min);
                // Стиль линии - пунктирная
                Minline.Line.Style = System.Drawing.Drawing2D.DashStyle.Dash;
                Maxline2.Line.Style = System.Drawing.Drawing2D.DashStyle.Dash;
                Minline.Line.Color = Color.Orange;
                Maxline2.Line.Color = Color.Orange;
                // Добавим линию в список отображаемых объектов
                zedGraphControl1.GraphPane.GraphObjList.Add(Minline);
                zedGraphControl1.GraphPane.GraphObjList.Add(Maxline2);

                // Нарисуем стрелку, указыающую на максимум функции
                // Координаты точки, куда указывает стрелка
                // Координаты привязаны к осям
                double xMaxend = maxx;
                double yMaxend = Y.Max();

                // Координаты точки начала стрелки
                double xstart = xMaxend + 0.05;
                double ystart = yMaxend + 0.05;

                // Рисование стрелки с текстом
                // Создадим стрелку
                ArrowObj arrow = new ArrowObj(xstart, ystart, xMaxend, yMaxend);

                // Добавим стрелку в список отображаемых объектов
                zedGraphControl1.GraphPane.GraphObjList.Add(arrow);

                // Напишем текст около начала стрелки
                // Координаты привязаны к осям
                TextObj text = new TextObj(Convert.ToString(Math.Round(Y.Max(), 5)), xstart, ystart);

                // Отключим рамку вокруг текста
                text.FontSpec.Border.IsVisible = false;

                // Добавим текст в список отображаемых объектов
                zedGraphControl1.GraphPane.GraphObjList.Add(text);
                ///Минимальный элемент
                double xMinend = minx;
                double yMinend = Y.Min();

                // Координаты точки начала стрелки
                 xstart = xMinend + 0.05;
                 ystart = yMinend + 0.05;

                // Рисование стрелки с текстом
                // Создадим стрелку
                 arrow = new ArrowObj(xstart, ystart, xMinend, yMinend);

                // Добавим стрелку в список отображаемых объектов
                zedGraphControl1.GraphPane.GraphObjList.Add(arrow);

                // Напишем текст около начала стрелки
                // Координаты привязаны к осям
                 text = new TextObj(Convert.ToString(Math.Round(Y.Min(), 5)), xstart, ystart);

                // Отключим рамку вокруг текста
                text.FontSpec.Border.IsVisible = false;

                // Добавим текст в список отображаемых объектов
                zedGraphControl1.GraphPane.GraphObjList.Add(text);
                ///-----------------------------------------

            }
        }
        private bool checkMethod(double a, double b, double x, double y, double size)
        {
            if (copyValue == null)
                throw new NullReferenceException("Пустая копия");
            if (a == copyValue[0] && b == copyValue[1] && x == copyValue[2] && y == copyValue[3] && size == copyValue[4])
            {

                if (checkBox1.Checked == true && activeDraw[0] == false)
                {
                    activeDraw[0] = true;
                    return false;
                }
                if (checkBox2.Checked == true && activeDraw[1] == false)
                {
                    activeDraw[1] = true;
                    return false;
                }
                if (checkBox3.Checked == true && activeDraw[2] == false)
                {
                    activeDraw[2] = true;
                    return false;
                }
                if (checkBox4.Checked == true && activeDraw[3] == false)
                {
                    activeDraw[3] = true;
                    return false;
                }
                if (checkBox5.Checked == true && activeDraw[4] == false)
                {
                    activeDraw[4] = true;
                    return false;
                }
                MessageBox.Show("Такой график уже построен");
                return true;
            }else
                return false;

        }

        private bool check(double a, double b,double x,double y, double size)
        {
            return a == copyValue[0] && b == copyValue[1] && x == copyValue[2] && y == copyValue[3] && size == copyValue[4];
        }

        /// <summary>
        /// на случай если введены новые значения след, каждый график-метод строится по новому
        /// необходимо запомнить значения
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="size"></param>
        private void copy(double a, double b, double x, double y, double size)
        {
            copyValue[0] = a;
            copyValue[1] = b;
            copyValue[2] = x;
            copyValue[3] = y;
            copyValue[4] = size;
        }
        private bool firstLaunched()
        {
            return activeDraw[0] == false && activeDraw[1] == false && activeDraw[2] == false && activeDraw[3] == false && activeDraw[4] == false;
        }

        //Вычисление функции методами-----------------------------------------------------------------------
        private void calcucaltion_DQ()
        {
            //Ввод и проверка ---------------------
            double[] nodeX = new double[1000];
            double[] nodeY = new double[1000];
            double a = 0, b = 0, size = 0;
            if(textBox1.Text!="")
            {
                string[] sNums = textBox1.Text.Split(' ');
                if(sNums.Length<5)
                {
                    MessageBox.Show("Недостаточно данных!");
                    return;
                }
                for(int i=0;i<sNums.Length;i++)
                {
                    for(int j=0;j<sNums[i].Length;j++)
                    {
                        if(sNums[i][j]>='a' && sNums[i][j]<='z')
                        {
                            MessageBox.Show("В поле есть посторонние символы");
                            return;
                        }
                    }
                }
                if(sNums[0]=="" || sNums[1]=="" || sNums[2]=="" || sNums[3]=="" || sNums[4]=="")
                {
                    MessageBox.Show("Недостаточно данных или поле пустое");
                    return;
                }
                a = Convert.ToDouble(double.Parse(sNums[0].Replace(".", ",")));
                 b = Convert.ToDouble(double.Parse(sNums[1].Replace(".", ",")));
                nodeX[0] = Convert.ToDouble(double.Parse(sNums[2].Replace(".", ",")));
                nodeY[0] = Convert.ToDouble(double.Parse(sNums[3].Replace(".", ",")));
                 size = Convert.ToInt32(sNums[4]);
            }else
            {
                MessageBox.Show("Поле ввода пустое!");
                return;
            }
            //Конец проверки-----------------------------------------------------
            
            //Не рисовать график дважды при тех же значениях
            if (firstLaunched() == false && allMethod.Checked==false)
            {
                if (checkMethod(a, b, nodeX[0], nodeY[0], size) == true && allMethod.Checked == false)
                {
                    return;
                }
                if (check(a, b, nodeX[0], nodeY[0], size) == false)
                {
                    if (activeDraw[0] == true)
                    {
                        zedGraphControl1.GraphPane.CurveList[0].Clear();
                    }

                    if (activeDraw[1] == true)
                    {
                        zedGraphControl1.GraphPane.CurveList[1].Clear();
                    }

                    if (activeDraw[2] == true)
                    {
                        zedGraphControl1.GraphPane.CurveList[2].Clear();
                    }

                    if (activeDraw[3] == true)
                    {
                        zedGraphControl1.GraphPane.CurveList[3].Clear();
                    }

                    if (activeDraw[4] == true)
                    {
                        zedGraphControl1.GraphPane.CurveList[4].Clear();
                    }
                    zedGraphControl1.GraphPane.GraphObjList.Clear();
                    activeDraw[0] = false;
                    activeDraw[1] = false;
                    activeDraw[2] = false;
                    activeDraw[3] = false;
                    activeDraw[4] = false;
                    copy(a, b, nodeX[0], nodeY[0], size);
                }

            }else
            {
                if(allMethod.Checked==false)
                {
                    if (checkBox1.Checked == true) activeDraw[0] = true;
                    if (checkBox2.Checked == true) activeDraw[1] = true;
                    if (checkBox3.Checked == true) activeDraw[2] = true;
                    if (checkBox4.Checked == true) activeDraw[3] = true;
                    if (checkBox5.Checked == true) activeDraw[4] = true;
                    copy(a, b, nodeX[0], nodeY[0], size);
                }

            }//----------------------------------------проверка графика----------------------------------

            //Вычисления------------------------------------
            double h = 0.0;
            h = (b - a) / size;
            if(checkBox1.Checked==true || (allMethod.Checked==true && active[0]==0))
            {
                methodEuler(ref nodeX, ref nodeY,h,size);
                inputList(nodeX, nodeY, size);
                if (allMethod.Checked==true)
                    active[0] = 1;
                return;
            }
            if(checkBox2.Checked==true || (allMethod.Checked == true && active[1]==0))
            {
                methodRunge_Kutt_second(ref nodeX,ref nodeY, h, size);
                inputList(nodeX, nodeY, size);
                if (allMethod.Checked == true)
                    active[1] = 1;
                return;
            }
            if(checkBox3.Checked==true || (allMethod.Checked == true && active[2] ==0))
            {
                methodRunge_Kutt_fourth(ref nodeX, ref nodeY, h, size);
                inputList(nodeX, nodeY, size);
                if (allMethod.Checked == true)
                    active[2] = 1;
                return;
            }

            if(checkBox4.Checked==true || (allMethod.Checked == true && active[3] ==0))
            {
                methodAdamsBashfort_fourth(ref nodeX, ref nodeY, h, size);
                inputList(nodeX, nodeY, size);
                if (allMethod.Checked == true)
                    active[3] = 1;
                return;
            }
            if(checkBox5.Checked==true || (allMethod.Checked == true && active[4] == 0))
            {
                methodAdamMulton_second(ref nodeX, ref nodeY, h, size);
                inputList(nodeX, nodeY,size);
                if (allMethod.Checked == true)
                    active[4] = 1;
                return;
            }
        }

        private void inputList(double[] x,double[] y,double size)
        {
            for (int i = 0; i <= size; i++)
            {
                X.Add(x[i]);
                Y.Add(y[i]);
            }
        }
        private void methodEuler(ref double[] x, ref double[] y,double h, double size)
        { 
            for (int i = 1; i <= size; i++)
            {
                x[i] = x[i - 1] + h;
                y[i] = y[i - 1] + h * function(x[i - 1], y[i - 1]);
            }
        }
        private void methodRunge_Kutt_second(ref double[] x,ref double[] y, double h, double size)
        {
            for (int i = 1; i <= size; i++)
            {
                x[i] = x[i - 1] + h;
                y[i] = y[i - 1] + h * function(x[i - 1], y[i - 1]);
                y[i] = y[i - 1] + (h / 2) * (function(x[i - 1], y[i - 1]) + function(x[i], y[i]));               
            }
        }

        private void methodRunge_Kutt_fourth(ref double[] x, ref double[] y,double h,double size)
        {
            for (int i = 1; i <= size; i++)
            {
                x[i] = x[i - 1] + h;
                y[i] = y[i - 1] + (h / 6.0) * (Runge_K(x[i - 1], y[i - 1], h));
            }
        }
        private double Runge_K(double x, double y, double h)
        {
            double k1 = function(x, y);
            double k2 = 2.0 * function(x + h / 2.0, y + (h / 2.0) * k1);
            double k3 = 2.0 * function(x + h / 2.0, y + (h / 2.0) * k2);
            double k4 = function(x + h, y + h * k3);
            return k1 + k2 + k3 + k4;
        }

        private void methodAdamsBashfort_fourth(ref double[] x, ref double[] y, double h, double size)
        {
            for (int i = 1; i <= 3; i++)
            {
                x[i] = x[i - 1] + h;
                y[i] = y[i - 1] + (h / 6) * (Runge_K(x[i - 1], y[i - 1], h));
              
            }
            for (int i = 4; i <= size; i++)
            {
                x[i] = x[i - 1] + h;
                y[i] = y[i - 1] + (h / 24) *calc(x,y,i); 
            }

        }

        private double calc(double[] x,double[] y,int i)
        {
            return (55 * function(x[i - 1], y[i - 1]) - 59 * function(x[i - 2], y[i - 2]) +
                    37 * function(x[i - 3], y[i - 3]) - 9 * function(x[i - 4], y[i - 4]));
        }

        private void methodAdamMulton_second(ref double[] x, ref double[] y, double h, double size)
        {
            x[1] = x[0] + h;
            y[1] = y[0] + (h / 6.0) * (Runge_K(x[0], y[0], h));
            for (int i = 2; i <= size; i++)
            {
                x[i] = x[i - 1] + h;
                y[i] = y[i - 1] + (h / 2) * (function(x[i], y[i]) + function(x[i - 1], y[i - 1]));
            }
        }

        private double function(double x, double y)
        {
            return 3 * Math.Sin(2 * y) + x; //y'=3sin(2y)+x Начальное условие y(0)=2 на отрезке [0,1] h=0.1;
        }
        //Вычисление функции методы-----------------------------------------------------------------------
     

        private void Button2_Click(object sender, EventArgs e)
        {
            textBox1.Clear();
            checkBox1.Checked = false;
            checkBox2.Checked = false;
            checkBox3.Checked = false;
            checkBox4.Checked = false;
            checkBox5.Checked = false;
            flag = false;
            for(int i=0;i<5;i++)
            {
                active[i] = 0;
                activeDraw[i] = false;
            }
            if (allMethod.Enabled == false)
                allMethod.Enabled = true;
            zedGraphControl1.GraphPane.CurveList.Clear();
            zedGraphControl1.GraphPane.GraphObjList.Clear();
            drawBegan();
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
        }

        public void drawBegan()
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            // Очистим список кривых на тот случай, если до этого сигналы уже были нарисованы
            myPane.CurveList.Clear();

            myPane.Title.Text = "Методы решения ОДУ: 3sin(2y)+x";
            myPane.XAxis.Title.Text = " X ";
            myPane.YAxis.Title.Text = " Y ";

            //точки
            RollingPointPairList list = new RollingPointPairList(1200);
            RollingPointPairList list2 = new RollingPointPairList(1200);
            RollingPointPairList list3 = new RollingPointPairList(1200);
            RollingPointPairList list4 = new RollingPointPairList(1200);
            RollingPointPairList list5 = new RollingPointPairList(1200);
            // Создадим кривую с названием "Sinc",
            // которая будет рисоваться голубым цветом (Color.Blue),
            // Опорные точки выделяться не будут (SymbolType.None)
            LineItem curve1 = myPane.AddCurve("Эйлер", list, Color.Blue, SymbolType.Square);
            LineItem curve2 = myPane.AddCurve("Рунге-Кутты 2 порядка", list2, Color.Green, SymbolType.Diamond);
            LineItem curve3 = myPane.AddCurve("Рунге-Кутты 4 порядка", list3, Color.Brown, SymbolType.Circle);
            LineItem curve4 = myPane.AddCurve("Адамс-Баншорт 4 порядка", list4, Color.Red, SymbolType.Triangle);
            LineItem curve5 = myPane.AddCurve("Адамс-Мультон 2 порядка", list5, Color.Pink, SymbolType.Plus);


            timer1.Interval = 150;
            // myPane.XAxis.Scale.Min = 0;
            // myPane.XAxis.Scale.Max = 2;
            // myPane.XAxis.Scale.MinorStep = 0;
            //  myPane.XAxis.Scale.MajorStep = 3.0;

            // Вызываем метод AxisChange(), чтобы обновить данные об осях.
            // В противном случае на рисунке будет показана только часть графика,
            // которая умещается в интервалы по осям, установленные по умолчанию
            zedGraphControl1.AxisChange();
            tickStart = Environment.TickCount;

        }

        private void CheckBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
                checkBox2.Enabled = false;
                checkBox3.Enabled = false;
                checkBox4.Enabled = false;
                checkBox5.Enabled = false;
                if (flag == false)
                    allMethod.Enabled = false;
            }  
            else
            {
                checkBox2.Enabled = true;
                checkBox3.Enabled = true;
                checkBox4.Enabled = true;
                checkBox5.Enabled = true;
                if (flag == false)
                    allMethod.Enabled = true;
            }         
        }

        private void CheckBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox2.Checked == true)
            {
                checkBox1.Enabled = false;
                checkBox3.Enabled = false;
                checkBox4.Enabled = false;
                checkBox5.Enabled = false;
                if (flag == false)
                    allMethod.Enabled = false;
            }
            else
            {
                checkBox1.Enabled = true;
                checkBox3.Enabled = true;
                checkBox4.Enabled = true;
                checkBox5.Enabled = true;
                if (flag == false)
                    allMethod.Enabled = true;
            }
        }

        private void CheckBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox3.Checked == true)
            {
                checkBox1.Enabled = false;
                checkBox2.Enabled = false;
                checkBox4.Enabled = false;
                checkBox5.Enabled = false;
                if (flag == false)
                    allMethod.Enabled = false;
            }
            else
            {
                checkBox1.Enabled = true;
                checkBox2.Enabled = true;
                checkBox4.Enabled = true;
                checkBox5.Enabled = true;
                if (flag == false)
                    allMethod.Enabled = true;
            }
        }

        private void CheckBox4_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox4.Checked == true)
            {
                checkBox1.Enabled = false;
                checkBox2.Enabled = false;
                checkBox3.Enabled = false;
                checkBox5.Enabled = false;
                if (flag == false)
                    allMethod.Enabled = false;
            }
            else
            {
                checkBox1.Enabled = true;
                checkBox2.Enabled = true;
                checkBox3.Enabled = true;
                checkBox5.Enabled = true;
                if (flag == false)
                    allMethod.Enabled = true;
            }
        }

        private void CheckBox5_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox5.Checked == true)
            {
                checkBox1.Enabled = false;
                checkBox2.Enabled = false;
                checkBox3.Enabled = false;
                checkBox4.Enabled = false;
                if (flag == false)
                    allMethod.Enabled = false;
            }
            else
            {
                checkBox1.Enabled = true;
                checkBox2.Enabled = true;
                checkBox3.Enabled = true;
                checkBox4.Enabled = true;
                if (flag == false)
                    allMethod.Enabled = true;
            }
        }

        private void AllMethod_CheckedChanged(object sender, EventArgs e)
        {
            if (allMethod.Checked == true)
            {
                checkBox1.Enabled = false;
                checkBox2.Enabled = false;
                checkBox3.Enabled = false;
                checkBox4.Enabled = false;
                checkBox5.Enabled = false;
            }
            else
            {
                checkBox1.Enabled = true;
                checkBox2.Enabled = true;
                checkBox3.Enabled = true;
                checkBox4.Enabled = true;
                checkBox5.Enabled = true;
            }
        }

        //Стрелка и значения по указателю мыши-----------------------------------
        private void ZedGraphControl1_MouseClick(object sender, MouseEventArgs e)
        {
            CurveItem curve;

            // Сюда будет сохранен номер точки кривой, ближайшей к точке клика
            int index;

            // Максимальное расстояние от точки клика до кривой в пикселях,
            // при котором еще считается, что клик попал в окрестность кривой.
            GraphPane.Default.NearestTol = 10;

            bool result = zedGraphControl1.GraphPane.FindNearestPoint(e.Location, out curve, out index);

            if (result)
            {
                // Максимально расстояние от точки клика до кривой не превысило NearestTol

                // Добавим точку на график, вблизи которой произошел клик
                PointPairList point = new PointPairList();

                point.Add(curve[index]);

                // Кривая, состоящая из одной точки. Точка будет отмечена синим кругом
                LineItem curvePount = zedGraphControl1.GraphPane.AddCurve("",
                    new double[] { curve[index].X },
                    new double[] { curve[index].Y },
                    Color.Blue,
                    SymbolType.Circle);

                //
                curvePount.Line.IsVisible = false;

                // Цвет заполнения круга - колубой
                curvePount.Symbol.Fill.Color = Color.Blue;

                // Тип заполнения - сплошная заливка
                curvePount.Symbol.Fill.Type = FillType.Solid;

                // Размер круга
                curvePount.Symbol.Size = 7;


                // Нарисуем стрелку, указыающую на максимум функции
                // Координаты точки, куда указывает стрелка
                // Координаты привязаны к осям
                double xMaxend = curve[index].X;
                double yMaxend = curve[index].Y;

                // Координаты точки начала стрелки
                double xstart = xMaxend + 0.05;
                double ystart = yMaxend + 0.05;

                // Рисование стрелки с текстом
                // Создадим стрелку
                ArrowObj arrow = new ArrowObj(xstart, ystart, xMaxend, yMaxend);

                // Добавим стрелку в список отображаемых объектов
                zedGraphControl1.GraphPane.GraphObjList.Add(arrow);

                // Напишем текст около начала стрелки
                // Координаты привязаны к осям
                TextObj text = new TextObj(Convert.ToString(Math.Round(yMaxend, 5)), xstart, ystart);
                TextObj text2 = new TextObj(Convert.ToString(Math.Round(xMaxend, 5)), xstart, ystart-0.05);

                // Отключим рамку вокруг текста
                text.FontSpec.Border.IsVisible = false;
                text2.FontSpec.Border.IsVisible = false;

                // Добавим текст в список отображаемых объектов
                zedGraphControl1.GraphPane.GraphObjList.Add(text);
                zedGraphControl1.GraphPane.GraphObjList.Add(text2);
                ///-----------------------------------------
            }
        }//Стрелка и значения по указателю мыши-----------------------------------
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.Threading;

namespace FfmpegDummy
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("ffmpegPlayer.dll")] 
        public static extern int fmpPlayer_Start(string filePath);

        [DllImport("ffmpegPlayer.dll")]
        public static extern void fmpPlayer_Stop();

        public MainWindow()
        {
            InitializeComponent();

           // var args = Environment.GetCommandLineArgs();
           // if (args.Count() > 1)
            {

                int retCode = 0;
                retCode = fmpPlayer_Start("");

                //Thread.Sleep(6000);
                //fmpPlayer_Stop();
            }
        }
    }
}

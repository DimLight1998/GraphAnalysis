using System;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using CefSharp;
using CefSharp.WinForms;
using Core;

namespace Visualizer
{
    public sealed partial class Visualizer : Form
    {
        private readonly ChromiumWebBrowser _chromium;
        private readonly Wrapper _wrapper = new Wrapper();

        private delegate void UiManipulation();

        private string _currentInformation;

        public Visualizer()
        {
            File.Delete("./Assets/graph.json");

            Font = SystemFonts.MenuFont;
            InitializeComponent();
            ClientSize = new Size(1280, 720);
            MinimumSize = new Size(1280, 720);

            _chromium = new ChromiumWebBrowser("file:///./Assets/splash.html")
            {
                Dock = DockStyle.Fill,
                BrowserSettings = new BrowserSettings
                {
                    FileAccessFromFileUrls = CefState.Enabled,
                    UniversalAccessFromFileUrls = CefState.Enabled
                }
            };

            tableLayoutPanel1.Controls.Add(_chromium, 1, 0);

            ClientSizeChanged += (sender, args) => { _chromium.Reload(); };

            var background = new Thread(() =>
            {
                using (var file = new StreamWriter("./Assets/conf.js"))
                {
                    file.Write("confStrength = -1");
                }

                // Update name list.
                _wrapper.Load();
                var movieNames = _wrapper.GetAllMovieNames().ToArray();

                comboBox2.Invoke(new UiManipulation(() =>
                {
                    foreach (var movieName in movieNames)
                    {
                        comboBox2.Items.Add(movieName);
                        comboBox3.Items.Add(movieName);
                        comboBox4.Items.Add(movieName);
                    }

                    button2.Enabled = true;
                    button3.Enabled = true;
                    button4.Enabled = true;
                    button5.Enabled = true;
                    button6.Enabled = true;
                    button7.Enabled = true;
                }));
            });

            background.Start();
        }

        /// <summary>
        /// Called to change the force of the representation.
        /// </summary>
        private void TrackBar1_ValueChanged(object sender, EventArgs e)
        {
            var value = trackBar1.Value;
            using (var file = new StreamWriter("./Assets/conf.js"))
            {
                file.Write($"confStrength = -{Math.Pow(1.03, value)}");
            }
            _chromium.Reload();
        }

        /// <summary>
        /// Called to compute shortest path.
        /// </summary>
        private void Button2_Click(object sender, EventArgs e)
        {
            var sourceName = comboBox2.Text;
            var destinationName = comboBox3.Text;

            if (string.IsNullOrEmpty(sourceName) || string.IsNullOrEmpty(destinationName))
            {
                MessageBox.Show(@"Please select a valid name", @"Error");
                return;
            }

            var ans = _wrapper.GetShortestPath(sourceName, destinationName);
            var path = string.Join(" -> ", ans.Item1);
            var weight = ans.Item2;

            var information = $@"Path is
{path}
Weight in total:
{weight}";
            _currentInformation = information;
            var informationBox = new InformationBox();
            informationBox.SetText(information);
            informationBox.Show();

            ComboBox1_SelectedIndexChanged(null, null);
        }

        /// <summary>
        /// Called to change the representation.
        /// </summary>
        private void ComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            var presentation = comboBox1.Text;
            if (string.IsNullOrEmpty(presentation))
            {
                MessageBox.Show(@"No presentation selected", @"Note");
                return;
            }

            switch (presentation)
            {
                case "Force-Directed Graph":
                    _chromium.Load("file:///./Assets/force_directed_graph.html");
                    break;
                case "Curved Links":
                    _chromium.Load("file:///./Assets/curved_links.html");
                    break;
                case "Blocks Graph":
                    _chromium.Load("file:///./Assets/blocks_gragh.html");
                    break;
                default:
                    return;
            }
        }

        /// <summary>
        /// Called to compute minimal spanning tree.
        /// </summary>
        private void Button3_Click(object sender, EventArgs e)
        {
            _currentInformation = "";
            var rootName = comboBox4.Text;

            if (string.IsNullOrEmpty(rootName))
            {
                MessageBox.Show(@"Please select a valid name", @"Error");
                return;
            }

            _wrapper.GetMinimalSpanningTree(rootName);

            ComboBox1_SelectedIndexChanged(null, null);
        }

        /// <summary>
        /// Called to compute strongly connected components
        /// </summary>
        private void Button6_Click(object sender, EventArgs e)
        {
            _currentInformation = "";
            try
            {
                var threshold = int.Parse(textBox1.Text);
                var ret = _wrapper.GetStronglyConnectedComponents(threshold);

                var sizeList = ret.Select(list => list.Count).ToList();
                sizeList.Sort();

                var information = $"Size of components: " + string.Join(", ", sizeList);
                _currentInformation = information;
                var informationBox = new InformationBox();
                informationBox.SetText(information);
                informationBox.Show();

                ComboBox1_SelectedIndexChanged(null, null);
            }
            catch (Exception exception) when
            (exception is ArgumentException ||
             exception is FormatException ||
             exception is OverflowException)
            {
                MessageBox.Show(@"Please input a valid value.", @"Error");
            }
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            var text = _currentInformation;
            var informationBox = new InformationBox();
            informationBox.SetText(text);
            informationBox.Show();
        }

        private void Button7_Click(object sender, EventArgs e)
        {
            _currentInformation = "";
            _wrapper.GetGraph();

            ComboBox1_SelectedIndexChanged(null, null);
        }

        /// <summary>
        /// Called to calculate betweenness.
        /// </summary>
        private void Button4_Click(object sender, EventArgs e)
        {
            _currentInformation = "";
            MessageBox.Show(@"Calculation started, the answer will be shown after finished.", @"Note");
            button4.Enabled = false;
            button5.Enabled = false;

            var sb = new StringBuilder();
            var background = new Thread(() =>
            {
                var answers = _wrapper.GetAllBetweenness();
                foreach (var answer in answers)
                {
                    sb.Append($"[{answer.Key}]: {answer.Value}").AppendLine();
                }

                _currentInformation = sb.ToString();

                button4.Invoke(new UiManipulation(() =>
                {
                    var infoBox = new InformationBox();
                    infoBox.SetText(_currentInformation);
                    infoBox.Show();

                    button4.Enabled = true;
                    button5.Enabled = true;
                }));
            });

            background.Start();
        }

        /// <summary>
        /// Called to calculate closeness.
        /// </summary>
        private void Button5_Click(object sender, EventArgs e)
        {
            _currentInformation = "";
            MessageBox.Show(@"Calculation started, the answer will be shown after finished.", @"Note");
            button4.Enabled = false;
            button5.Enabled = false;

            var sb = new StringBuilder();
            var background = new Thread(() =>
            {
                var answers = _wrapper.GetAllCloseness();
                foreach (var answer in answers)
                {
                    sb.Append($"[{answer.Key}]: {answer.Value}").AppendLine();
                }

                _currentInformation = sb.ToString();

                button4.Invoke(new UiManipulation(() =>
                {
                    var infoBox = new InformationBox();
                    infoBox.SetText(_currentInformation);
                    infoBox.Show();

                    button4.Enabled = true;
                    button5.Enabled = true;
                }));
            });

            background.Start();
        }
    }
}
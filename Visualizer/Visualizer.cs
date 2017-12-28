using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using CefSharp;
using CefSharp.WinForms;
using Core;

namespace Visualizer
{
    public sealed partial class Visualizer : Form
    {
        private readonly ChromiumWebBrowser _chromium;
        private List<String> _movieNames;
        private readonly Wrapper _wrapper = new Wrapper();

        public Visualizer()
        {
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

            using (var file = new StreamWriter("./Assets/conf.js"))
            {
                file.Write("confStrength = -1");
            }

            // Update name list.
            _wrapper.Load();
            _movieNames = _wrapper.GetAllMovieNames();

            var movieNames = _movieNames.ToArray();

            foreach (var movieName in movieNames)
            {
                comboBox2.Items.Add(movieName);
                comboBox3.Items.Add(movieName);
                comboBox4.Items.Add(movieName);
            }
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

            var ans = _wrapper.GetShortestPath(sourceName, destinationName);
            var path = string.Join(" -> ", ans.Item1);
            var weight = ans.Item2;

            textBox2.Text = $"Path is\n{path}\nWeight in total: {weight}";
            ComboBox1_SelectedIndexChanged(null, null);
        }

        /// <summary>
        /// Called to change the representation.
        /// </summary>
        private void ComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            var presentation = comboBox1.Text;
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
            var rootName = comboBox4.Text;
            _wrapper.GetMinimalSpanningTree(rootName);

            ComboBox1_SelectedIndexChanged(null, null);
        }

        /// <summary>
        /// Called to compute strongly connected components
        /// </summary>
        private void Button6_Click(object sender, EventArgs e)
        {
            textBox2.Clear();
            var threshold = int.Parse(textBox1.Text);
            var ret = _wrapper.GetStronglyConnectedComponents(threshold);

            var sizeList = ret.Select(list => list.Count).ToList();
            sizeList.Sort();

            textBox2.Text = "Size of components: " + string.Join(", ", sizeList);

            ComboBox1_SelectedIndexChanged(null, null);
        }
    }
}
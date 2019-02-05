using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BBBallStageConverter
{
	class Program
	{
		static void Main(string[] args)
		{
			if (args.Length != 2)
			{
				var program_name = Environment.GetCommandLineArgs()[0];
				Console.WriteLine("usage: " + program_name + " outfile infile");
				Environment.Exit(1);
			}

			try
			{
				var stage = new IntermediateStage(Environment.GetCommandLineArgs()[2]);
				stage.Write(Environment.GetCommandLineArgs()[1]);
			}
			catch (Exception e)
			{
				Console.WriteLine("error: " + e.Message);
				Environment.Exit(1);
			}
		}
	}
}

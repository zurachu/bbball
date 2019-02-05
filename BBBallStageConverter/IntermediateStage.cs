using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace BBBallStageConverter
{
	class IntermediateStage
	{
		static readonly int height = 11;
		static readonly int writeDataPerLine = 8;

		string name;
		List<List<int>> blocks;
		int width;
		int goalX;
		int goalY;

		public IntermediateStage(string path)
		{
			name = Path.GetFileNameWithoutExtension(path);

			var reader = new StreamReader(path);

			var lines = new List<string>();
			for (var i = 0; i < height; i++)
			{
				lines.Add(reader.ReadLine());
			}
			lines.Reverse();
			width = lines.ConvertAll(s => s.Length).Max();

			blocks = new List<List<int>>();
			for (var y = 0; y < height; y++)
			{
				var line = lines[y];
				var blocksByLine = new List<int>();
				for (var x = 0; x < width; x++)
				{
					if (x < line.Length)
					{
						switch (line[x])
						{
							case ' ':
								blocksByLine.Add(0);
								break;
							case 'G':
								blocksByLine.Add(1);
								goalX = x;
								goalY = y;
								break;
							default:
								blocksByLine.Add(1);
								break;
						}
					}
					else
					{
						blocksByLine.Add(0);
					}
				}
				blocks.Add(blocksByLine);
			}

			reader.Close();
		}

		public void Write(string path)
		{
			var writer = new StreamWriter(path);

			writer.WriteLine("#include \"stage.h\"");
			writer.WriteLine();
			writer.WriteLine("#include \"ArraySize.h\"");
			writer.WriteLine();
			writer.WriteLine("static unsigned short s_stage_data[] = {");

			for (var x = 0; x < width; x++)
			{
				if (x % writeDataPerLine == 0)
				{
					writer.Write("\t");
				}
				writer.Write("0x{0,0:X3}, ", StageData(x));
				if (x % writeDataPerLine == writeDataPerLine - 1 && x < width - 1)
				{
					writer.WriteLine();
				}
			}
			writer.WriteLine();

			writer.WriteLine("};");
			writer.WriteLine();
			writer.WriteLine("struct Stage const g_" + name + " = {");
			writer.WriteLine("\ts_stage_data,");
			writer.WriteLine("\tARRAY_SIZE(s_stage_data),");
			writer.WriteLine("\t{0}, {1}", goalX, goalY);
			writer.WriteLine("};");

			writer.Close();
		}

		int StageData(int x)
		{
			if (x >= width)
			{
				return 0;
			}

			var ret = 0;
			for (var y = 0; y < height; y++)
			{
				ret |= blocks[y][x] << y;
			}
			return ret;
		}
	}
}

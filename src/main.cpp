/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/12 20:09:10 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/25 21:27:45 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Factory.hpp>

#define O_FLAG 1
#define B_FLAG 2
#define I_FLAG 4

int					read_flag(int & argc, char const  ** & argv)
{
	static std::string const _o("-o");
	static std::string const _b("-b");
	static std::string const _u("--usage");

	if (argc == 1)
		return 0;
	argc--;
	argv++;
	if (_o.compare(*argv) == 0)
		return O_FLAG;
	if (_b.compare(*argv) == 0)
		return B_FLAG;
	if (_u.compare(*argv) == 0)
	{
		std::cout <<"Usage : ./avm [-o file.bytecode] [file.avm] [-b file.bytecode]" << std::endl;
		exit(0);
	}
	argc++;
	argv--;
	return I_FLAG;
}

void				manage_file(char const *file,
							std::ofstream * &ofs,
							std::ifstream * &ifs,
							int flag)
{
	try
	{
		if (ofs && !(flag & I_FLAG))
		{
			ofs->close();
			delete ofs;
			ofs = NULL;
		}
		if (ifs)
		{
			ifs->close();
			delete ifs;
			ifs = NULL;
		}
		if (flag && !file)
			throw AvmException("no file specified");
		if (flag & (I_FLAG | B_FLAG))
			ifs = new std::ifstream(file);
		else if (flag & O_FLAG)
			ofs = new std::ofstream(file);
		if ((ifs && !ifs->good()) || (ofs && !ofs->good()))
			throw AvmException("file doesn't exist or can't be opened");
	}
	catch (std::exception & e)
	{
		std::cerr << "Avm file error : " << file << " -> " << e.what() << std::endl;
		exit(-1);
	}
}



int 				main(int argc, char const *argv[])
{
	Factory				factory;
	Avm					avm;
	std::ofstream		*ofs = NULL;
	std::ifstream		*ifs = NULL;
	int					flag;
	int					f;
	
	flag = 0;
	try
	{
		while (argc > 0)
		{
			f = read_flag(argc, argv);
			argc--;
			argv++;
			if (((f & (I_FLAG | B_FLAG)) && ifs) || argc == 0) //there is a file waiting or no more file
			{
				if ((flag & B_FLAG) && ifs)
					avm.loadBinary(*ifs);
				else
					factory.assemble_file(ifs ? *ifs : std::cin, avm, ofs);
				if (!ofs)
					avm.run();
				flag = 0;
			}
			flag |= f;
			manage_file(argc > 0 ? *argv : NULL, ofs, ifs, f);
		}
	} 
	catch (std::exception & e)
	{
		std::cerr << "Avm line " << (avm.get_line() + 1) << " :  " << e.what() << std::endl;
		return -1;
	}
	return 0;
}
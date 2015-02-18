/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/12 20:09:10 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/17 11:42:13 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Factory.hpp>

#define O_FLAG 1
#define B_FLAG 2
#define I_FLAG 4

int					read_flag(int & argc, char const  ** & argv)
{
	argc--;
	argv++;
	if (std::string("-o").compare(*argv) == 0)
		return O_FLAG;
	if (std::string("-b").compare(*argv) == 0)
		return B_FLAG;
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
		}
		if (ifs)
		{
			ifs->close();
			delete ifs;
		}
		if (flag & (I_FLAG | B_FLAG))
			ifs = new std::ifstream(file);
		else if (flag & O_FLAG)
			ofs = new std::ofstream(file);
	}
	catch (std::exception & e)
	{
		std::cout << "Avm file error : " << file << " " << e << std::endl;
	}
}

void				print_usage()
{
	std::cout 
	<< "Usage : "
	<< argv[0]
	<< " [-o file.bytecode] [file.avm] [-b file.bytecode]" 
	<< std::endl;
	exit();
}

int 				main(int argc, char const *argv[])
{
	Factory				factory;
	Avm					avm;
	std::ofstream		*ofs = NULL;
	std::ifstream		*ifs = NULL;
	int					flag;
	
	if (argc == 1)
		print_usage();
	try
	{
		while (--argc)
		{
			flag |= read_flag(argc, argv);
			if ((flag & I_FLAG) && ((flag & (O_FLAG | B_FLAG)) || argc == 0))
			{
				if (!(flag & B_FLAG))
					manage_file(*argv, ofs, ifs, flag);	
				factory.assemble_file(ifs, avm, ofs);
				if (!(flag & O_FLAG))
					avm.run();
				flag &= ~(O_FLAG | I_FLAG);
			}
			if (flag & B_FLAG)
			{
				manage_file(*argv, ofs, ifs, flag);
				avm.loadBinary(ifs);
				avm.run();
				flag &= ~B_FLAG;
			}
			else if (flag & O_FLAG)
				manage_file(*argv, ofs, ifs, flag);
			argc--;
			argv++;
		}
		manage_file("", ofs, ifs, 0);	
	} 
	catch (std::exception & e)
	{
		std::cout << "Avm " << *argv << " " << avm.get_line() << e << std::endl;
	}
	return 0;
}
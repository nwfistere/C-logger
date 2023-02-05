#include "logger_group_test.hpp"

BOOST_AUTO_TEST_CASE(SetFormatTest)
{
    std::unique_ptr<std::ofstream> ofile = std::make_unique<std::ofstream>("./output-file.txt", std::ios::app);
    // *(ofile.get()) << "Start" << std::endl;

    // unmovable_logger<std::ostream> cout_logger(&std::cout);
    movable_logger<std::ofstream> file_logger1(std::move(ofile), TRACE, "%^m\n");
    movable_logger<std::ofstream> file_logger(std::make_unique<std::ofstream>("./output-file2.txt"), TRACE, "%^m\n");

    // std::unique_ptr<movable_logger<std::ofstream>> pfile_logger1 = std::make_unique<movable_logger<std::ofstream>>(file_logger1);

    // logger_group::get().add_logger(std::make_unique<base_logger>(file_logger));
    // logger_group::get().add_logger(std::move(pfile_logger1));

    logger_group::get().add_logger(
        std::unique_ptr<base_logger>(new movable_logger<std::ofstream>(std::make_unique<std::ofstream>("./output-file2.txt"), TRACE, "%^m\n"))
    );

    logger_group::get().log(DEBUG, "test\n\n\n\n");
}
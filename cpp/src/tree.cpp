/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Dave Hairapetian  			    *
* Date: 18.07.2023						    	*
* Description: Source file for Tree            	*
* Version: 1.2                                  *
* Status: Approved                              *
*************************************************/

#include <iostream>  //cout, cerr, endl
#include <string>    //string
#include <vector>    //vector
#include <exception> //struct exception
#include <dirent.h>  //DIR, struct dirent
#include <cstring>   //strcmp

#include "tree.hpp"

using std::cout;
using std::endl;

namespace dev
{

/************************************ Classes Declarations *******************************************/
class Directory : public IFile
{
public:
    explicit Directory(const char *path);
    Directory(const Directory& other_);
    virtual ~Directory();
    virtual void Print(unsigned int depth) const;
    virtual Directory *Clone() const;

private:
    std::string m_dir_name;
    std::vector<IFile *> m_tree_vector;
};

class File : public IFile
{
public:
    explicit File(const char *path);
    File(const File& other_);
    virtual ~File();
    virtual void Print(unsigned int depth) const;
    virtual File *Clone() const;

private:
    std::string m_file_name;
};

/************************************ IFile *******************************************/

IFile::IFile() {}
IFile::~IFile() {}
IFile::IFile(const IFile& other_) {(void)other_;}

/************************************ Directory *******************************************/
Directory::Directory(const char* path): m_dir_name(path) 
{
    struct dirent *s_dirent = NULL;
    DIR *dir_ent = opendir(path);

    if(NULL != dir_ent)
    {
        while (NULL != (s_dirent = readdir(dir_ent)))
        {
            try
            {
                if(DT_REG == s_dirent->d_type)
                {
                    File *new_file = new File(s_dirent->d_name);
                    m_tree_vector.push_back(new_file);
                }
                else if(DT_DIR == s_dirent->d_type && 0 != strcmp(s_dirent->d_name, ".") && 0 != strcmp(s_dirent->d_name, ".."))
                {
                    std::string sub_dir = std::string(path) + "/" + std::string(s_dirent->d_name);
                    
                    Directory *new_dir = new Directory(sub_dir.c_str());
                    m_tree_vector.push_back(new_dir);
                }
            }
            catch(const std::exception& e)
            {
                std::vector<IFile*>::const_iterator it = m_tree_vector.begin();
                for (; it != m_tree_vector.end(); ++it)
                {
                    delete(*it);
                }
                std::cerr << "Error: " << e.what() << endl; 
                throw;
            }
        }
        closedir(dir_ent);
    }
}

Directory::Directory(const Directory& other_): IFile(other_), m_dir_name(other_.m_dir_name)
{
    std::vector<IFile*>::const_iterator it = other_.m_tree_vector.begin();

    for(;it != other_.m_tree_vector.end(); ++it)
    {
        m_tree_vector.push_back((*it)->Clone());
    }
}

Directory::~Directory()
{
    std::vector<IFile*>::const_iterator it = m_tree_vector.begin();
    for (; it != m_tree_vector.end(); ++it)
    {
        delete(*it);
    }
}

void Directory::Print(unsigned int depth) const
{
    for (size_t i = 0; i < depth; i++)
    {
    std::cout << "\u2502" << "   ";
    }

    std::cout << "\u251c\u2500\u2500\x1B[1;34m" << m_dir_name.substr(m_dir_name.find_last_of("/\\") + 1) << "\x1B[0m" << std::endl;

    std::vector<IFile*>::const_iterator it = m_tree_vector.begin();
    for (; it != m_tree_vector.end(); ++it)
    {
        (*it)->Print(depth + 1);
    }
}

Directory *Directory::Clone() const
{
    return new Directory(*this);
}

/************************************ File *******************************************/
File::File(const char *path):m_file_name(path) {}

File::File(const File& other_): IFile(other_), m_file_name(other_.m_file_name) {}

File::~File() {}

void File::Print(unsigned int depth) const
{
    for (size_t i = 0; i < depth; i++)
    {
    std::cout << "\u2502" << "   ";
    }
    
    std::cout  << "\u251c\u2500\u2500 " << m_file_name << std::endl;
}

File *File::Clone() const
{
    return new File(*this);
}

/************************************ Tree *******************************************/
Tree::Tree(const char *path): m_root_path(new Directory(path)) {}

Tree::Tree(const Tree& other_):  m_root_path(other_.m_root_path->Clone()) {}

Tree::~Tree()
{
    delete m_root_path;
}

void Tree::Print(unsigned int depth) const
{
    if(NULL != m_root_path)
    {
        cout << "\x1B[1;34m.\x1B[0m" << endl;
        m_root_path->Print(depth);
    }
}

Tree *Tree::Clone() const
{
    return new Tree(*this);
}

} //namespace dev
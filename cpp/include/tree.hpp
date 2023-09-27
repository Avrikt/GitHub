/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Dave Hairapetian  			    *
* Date: 18.07.2023						    	*
* Description: Header file for Tree            	*
* Version: 1.2                                  *
* Status: Approved                              *
*************************************************/

#ifndef MY_PROJECT_TREE_HPP
#define MY_PROJECT_TREE_HPP

namespace dev
{
class IFile
{
public:
    IFile();
    virtual ~IFile();
    virtual void Print(unsigned int depth = 0) const = 0;
    virtual IFile *Clone() const = 0;

protected:
    IFile(const IFile &other);
    IFile& operator=(const IFile& other);
};

class Tree : public IFile
{
public:
    explicit Tree(const char *path);
    virtual ~Tree();
    virtual void Print(unsigned int depth = 0) const;
    virtual Tree *Clone() const;

protected:
    Tree(const Tree& other_);

private:
    IFile *m_root_path;
};
} // namespace dev

#endif // MY_PROJECT_TREE_HPP
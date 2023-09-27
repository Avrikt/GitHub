/********************************************
* Developer: Avri Kehat							*
* Reviewed by:									*
* Date: 30.08.2023								*
* Description: Test file for dispatcher			*
* Version: 1.1                              	*
* Status: Sent                              	*
*************************************************/

/************************************LIBRARIES**************************************************/

#include "dispatcher.hpp"
#include "utilities.hpp"

class ViewWindow
{
public:
	void OnEvent(std::string msg)
	{
		std::cout << "ViewWindow received: " << msg << std::endl;
	}

	void OnDeath()
	{
		std::cout << "ViewWindow died" << std::endl;
	}
};

class DataModel
{
public:
	void OnEvent(std::string msg)
	{
		std::cout << "DataModel received: " << msg << std::endl;
	}

	void OnDeath()
	{
		std::cout << "DataModel died" << std::endl;
	}
};

class ControlWindow
{
public:
	void OnEvent(std::string msg)
	{
		std::cout << "ControlWindow received: " << msg << std::endl;
	}

	void OnDeath()
	{
		std::cout << "ControlWindow died" << std::endl;
	}
};

using namespace dev;

static void TestOneBroadcast(void);

static void TestTwoBroadcasts(void);

static void TestThreeBroadcasts(void);

int main(void)
{
	TestOneBroadcast();

	TestTwoBroadcasts();

	TestThreeBroadcasts();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/
static void TestOneBroadcast(void)
{
	HEADER(One Broadcast);

	Dispatcher<std::string> *dis = new Dispatcher<std::string>;

	ViewWindow vw;

	Callback<std::string, ViewWindow> callback(&vw, &ViewWindow::OnEvent, &ViewWindow::OnDeath);

	dis->Subscribe(callback);
	dis->Notify("Test 1");

	delete dis;
}

static void TestTwoBroadcasts(void)
{
	HEADER(Two Broadcasts);

	Dispatcher<std::string> *dis = new Dispatcher<std::string>;

	ViewWindow vw;
	DataModel dm;

	Callback<std::string, ViewWindow> callback(&vw, &ViewWindow::OnEvent, &ViewWindow::OnDeath);
	Callback<std::string, DataModel> callback2(&dm, &DataModel::OnEvent, &DataModel::OnDeath);

	dis->Subscribe(callback);
	dis->Subscribe(callback2);

	dis->Notify("Test 1");
	dis->Notify("Test 2");

	delete dis;
}

static void TestThreeBroadcasts(void)
{
	Dispatcher<std::string> *dis = new Dispatcher<std::string>;

	ViewWindow vw;
	DataModel dm;
	ControlWindow cm;

	Callback<std::string, ViewWindow> callback(&vw, &ViewWindow::OnEvent, &ViewWindow::OnDeath);
	Callback<std::string, DataModel> callback2(&dm, &DataModel::OnEvent, &DataModel::OnDeath);
	Callback<std::string, ControlWindow> cb3(&cm, &ControlWindow::OnEvent, &ControlWindow::OnDeath);

	dis->Subscribe(callback);
	dis->Subscribe(callback2);
	dis->Subscribe(cb3);

	HEADER(Three Subscribed);
	dis->Notify("Test 1");
	dis->Notify("Test 2");

	HEADER(One Unsubscribed);
	cb3.Unsubscribe();
	dis->Notify("Test 3");

	HEADER(Unsubscribe All);
	callback.Unsubscribe();
	callback2.Unsubscribe();

	delete dis;
}
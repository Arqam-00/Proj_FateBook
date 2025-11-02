#include"Fate_Book.h"
#include<fstream>

Fate_Book::Fate_Book():current_user(nullptr) {}
Fate_Book::~Fate_Book(){
	for (auto it = Users_.begin(); it != Users_.end(); ++it) {
		delete* it;
	}
}
bool Fate_Book::Initialize(){
	load_from_file();
	return true;

}
User* Fate_Book::CreateUser(const string& Name, const string& Email, const string& Password,
	const string& Location, char Gender, int Age, bool IsPublic, const Date& CreatedAt){
	if (Check_By_Email(Email)) {
		//email already in use
		// should not go further then here
	}
	User* temp= new User("assets/default.png", Name, Email, Password, Location, Gender, Age, IsPublic, CreatedAt);
	return temp;
}

DoublyList<User*>* Fate_Book::GetUsers(){

	return &Users_;
}
PostManager* Fate_Book::GetPostManager() { return &PostMgr_; }
void Fate_Book::Signup(){
	string _name, _email, _password;
	char _gender = 'U';
	int _age = 0;
	bool _ispublic = true;
	Date _CreatedAt;
	//take thesse inputs 
	if (Check_By_Email(_email)) {
		//email already in use
		// should not go further then here
	}
	User* _user = CreateUser(_name, _email, _password, "None", _gender, _age, _ispublic, _CreatedAt);
	Users_.push_back(_user);
	current_user = _user;
}

User* Fate_Book::Check(const string& email, const string& password) {
	for (auto it = Users_.begin(); it != Users_.end(); ++it) {
		User* user = *it;
		if (user->GetEmail() == email && user->CheckPassword(password)) return user;
	}
	return nullptr;
}
void Fate_Book::Login(){
	string email, password;
	//take input from user here
	User* new_user= Check(email, password);
	if (new_user)current_user = new_user;
	else {
		
		//          No Such account or password is wrong
	}
}
void Fate_Book::Logout(){
	if(current_user)current_user = nullptr;
}
void Fate_Book::DeleteUser(){
	if (!current_user) { return; }
	DoublyList<User*> f = current_user->GetFriendsList();
	for (auto it = f.begin(); it != f.end(); ++it) {
		User* friend_user = *it;
		friend_user->RemoveFriend(current_user);
	}

	for (auto it = Users_.begin(); it != Users_.end(); ++it) {
		if (*it == current_user) {
			Users_.erase_at(it);
			break;
		}
	}
	delete current_user;
	current_user = nullptr;
	write_into_file();

}
User* Fate_Book::Check_By_Email(const string& email){
	
	for (auto it = Users_.begin(); it != Users_.end(); ++it) {
		User* user = *it;
		if (user->GetEmail() == email) return user;
	}
	return nullptr;
}

void Fate_Book::OpenFeed(){
	
}

User* Fate_Book::idToUser(int id) {
	for (auto it = Users_.begin(); it != Users_.end(); ++it) {
		if ((*it)->id == id)
			return *it;
	}
	return nullptr;
}


void Fate_Book::load_from_file() {
	ifstream userFile("users.txt");
	ifstream postFile("posts.txt");
	ifstream friendFile("friends.txt");

	while (true) {
		User* u = new User();
		int isPublicFlag;
		if (!(userFile >> u->id >> u->Name >> u->Email >> u->Password >> u->Gender >>
			u->Age >> u->Location >> isPublicFlag >> u->PfpPath >>
			u->CreatedAt.Day >> u->CreatedAt.Month >> u->CreatedAt.Year))
			break;

		u->IsPublic = (isPublicFlag != 0);
		Users_.push_back(u);

		if (u->id >= User::NextID)
			User::NextID = u->id + 1;
	}

	
	while (true) {
		Post* p = new Post();
		int uid;
		if (!(postFile >> p->PostID >> uid >> p->Text >> p->TimeStamp >> p->LikeCount))
			break;

		User* owner = idToUser(uid);
		if (owner)
			owner->Posts.push_back(p);
		else
			delete p; 
	}

	while (true) {
		int id1, id2;
		if (!(friendFile >> id1 >> id2))
			break;

		User* u1 = idToUser(id1);
		User* u2 = idToUser(id2);
		if (u1 && u2) {
			u1->AddFriend(u2);
			u2->AddFriend(u1);
		}
	}

	userFile.close();
	postFile.close();
	friendFile.close();


}
void Fate_Book::write_into_file() {
	ofstream userFile("users.txt");
	ofstream postFile("posts.txt");
	ofstream friendFile("friends.txt");

	

	for (auto it = Users_.begin(); it != Users_.end(); ++it) {
		User* u = *it;

	
		userFile << u->id << " "
			<< u->Name << " "
			<< u->Email << " "
			<< u->Password << " "
			<< u->Gender << " "
			<< u->Age << " "
			<< u->Location << " "
			<< u->IsPublic << " "
			<< u->PfpPath << " "
			<< u->CreatedAt.Day << " "
			<< u->CreatedAt.Month << " "
			<< u->CreatedAt.Year << "\n";


		for (auto pit = u->Posts.begin(); pit != u->Posts.end(); ++pit) {
			Post* p = *pit;
			postFile << p->PostID << " "
				<< u->id << " "
				<< p->Text << " "
				<< p->TimeStamp << " "
				<< p->LikeCount << "\n";
		}
		auto frnds = u->GetFriendsList();

		for (auto fit = frnds.begin(); fit != frnds.end(); ++fit) {
			User* f = *fit;
			if (u->id < f->id) 
				friendFile << u->id << " " << f->id << "\n";
		}
	}

	userFile.close();
	postFile.close();
	friendFile.close();
}
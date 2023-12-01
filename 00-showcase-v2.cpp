#include "openai.hpp"

#include <iostream>
#include <string>

std::string reply(std::string quest){
	nlohmann::json chat;
	chat["model"] = "gpt-4";
	chat["messages"] ={{{"role", "user"}, {"content", quest}}};
	chat["max_tokens"] = 1000;
	chat["temperature"] = 0;
	nlohmann::json reply = openai::chat().create(chat);
	std::string rep = std::string(reply["choices"][0]["message"]["content"]);
	return rep; 
}

template<class T>
class node{
public:
	T data;
	node<T>* next;
	node(T data_in, node<T>* next_):
		data(data_in), next(next_){
	}
	~node(){}
};

template<class T>

class route{
private:
	int size;
	node<T>* head;
	node<T>* tail;
public:
	route();
	~route();
	void addNode(T data_in);
	node<T>* get_head(){return head;}
	node<T>* get_tail(){return tail;}	
	int get_size(){return size;}
};

class guide{
private:
	int days;
	std::string con;
	std::string pcon;
	route<std::string>* place;
	route<std::string>* content;
	route<int>* cost;
public:
	guide(int day);
	~guide();
	void insert(int d, std::string p, std::string c, int o);
	int get_day(){return days;}
	std::string get_con(){return con;}
	std::string get_pcon(){return pcon;}
	void print();
	void init(std::string pl, std::string co);
};

template<class T>
route<T>::route(){
	size = 0;
	head = NULL;
	tail = NULL;
}

template<class T>
route<T>::~route(){
	node<T>* tmp = head;
	for(int i = 0; i < size; i++){
		tmp = tmp -> next;
		delete head;
		head = tmp;
	}
}

template <class T>
void route<T>::addNode(T data_in){
	node<T>* nn = new node<T>(data_in, NULL);
	if(size == 0){
		head = nn;
		tail = head;
	}
	else{
		tail -> next = nn;
		tail = nn;
	}
	size++;
	return;
}

guide::guide(int day){
	days = day;
	place = new route<std::string>[day];
	content = new route<std::string>[day];
	cost = new route<int>[day];
}

guide::~guide(){
	delete[] place;
	delete[] content;
	delete[] cost;
}

void guide::init(std::string pl, std::string co){
	con = pl;
	pcon = co;
	return;
}

void guide::insert(int d, std::string p, std::string c, int o){
	place[d].addNode(p);
	content[d].addNode(c);
	cost[d].addNode(o);
	return;
}

void guide::print(){
	for(int i = 0; i < days; i++){
		node<std::string>* n1 = place[i].get_head();
		node<std::string>* n2 = content[i].get_head();
		node<int>* n3 = cost[i].get_head();
		for(int j = 0; j < place[i].get_size(); j++){
			std::cout << n1->data << " || " << n2->data << " || " << n3->data << "	";
			n1 = n1->next;
			n2 = n2->next;
			n3 = n3->next;
		}
		std::cout << std::endl;
	}
	return;
}


void extractCountriesAndContents(const std::string& input, std::vector<std::string>& countries, std::vector<std::string>& contents) {
    size_t position = 0;
	
    while (1) {
        // 나라 찾기
        size_t countryStart = input.find('.', position)+2;
		if (countryStart == std::string::npos){
            break;
		}		
        size_t countryEnd = input.find(':', countryStart);
        if (countryEnd == std::string::npos){
            break;
		}
        // 내용 찾기
        size_t contentStart = countryEnd+2;  // \n\n 다음으로 이동
        size_t contentEnd = input.find("\n\n", contentStart);
        if (contentEnd == std::string::npos){
            contentEnd = input.length();
			countries.push_back(input.substr(countryStart, countryEnd - countryStart));
			contents.push_back(input.substr(contentStart, contentEnd - contentStart));
			break;
		}	
        // 결과에 추가
        countries.push_back(input.substr(countryStart, countryEnd - countryStart));
        contents.push_back(input.substr(contentStart, contentEnd - contentStart));

        // 다음 검색을 위해 위치 갱신
        position = contentEnd + 2;  // \n\n 다음으로 이동
    }
}

int main() {
    openai::start(); // Will use the api key provided by `OPENAI_API_KEY` environment variable
    // openai::start("your_API_key", "optional_organization"); // Or you can handle it yourself
	std::cout << "가고 싶은 여행지가 있나요?" << std::endl;
	int t1;
	std::cout << "1. 예		2. 아니오"<<std::endl;
	std::cin >> t1;
	std::string place;
	std::string placeq;
	std::string coun;
	std::string counc;
	if(t1 == 1){
		std::cout << "가고 싶은 여행지를 입력해주세요." << std::endl;
		std::cin >> place;
		coun = place;
		place += "을 여행지로 추천하는 내용에 대해 130자 이내로 알려줘";
		place = reply(place);
		counc = place;
	}
	else if(t1 == 2){
		int t2, t3, t4;
		std::cout << "당신에게 맞는 여행지를 추천해드리겠습니다." << std::endl;
		std::cout << "힐링과 액티비티 중 무엇을 더 선호하시나요?" << std::endl;
		std::cout << "1. 힐링		2. 액티비티 " << std::endl;
		std::cin >> t2;
		std::cout << "맛집 탐방과 명소 탐방 중 무엇을 더 선호하시나요?" << std::endl;
		std::cout << "1. 맛집 탐방		2. 명소 탐방?" << std::endl;
		std::cin >> t3;
		std::cout << "자연 경관과 인공 경관 중 무엇을 더 선호하시나요?" << std::endl;
		std::cout << "1. 자연 경관		2. 인공 경관" << std::endl;
		std::cin >> t4;
		std::string place1, place2, place3;
		place1 = "저는 ";
		place2 = "보다 ";
		place3 = "을 선호합니다.";
		placeq += place1;
		placeq += (t2==1? "액티비티":"힐링");
		placeq += place2;
		placeq += (t2==1? "힐링":"액티비티");
		placeq += place3;
		placeq += place1;
		placeq += (t3==1? "명소 탐방":"맛집 탐방");
		placeq += place2;
		placeq += (t3==1? "맛집 탐방":"명소 탐방");
		placeq += place3;
		placeq += place1;
		placeq += (t4==1? "인공 경관":"자연 경관");
		placeq += place2;
		placeq += (t4==1? "자연 경관":"인공 경관");
		placeq += place3;
		placeq += "저에게 맞는 해외 여행할 만한 나라 3곳을 추천해주세요. 나라에 대한 소개는 각 130자 이내로 해주세요";
	//	std::cout << placeq << std::endl;
		while(1){
			std::string rep = reply(placeq);
			std::cout << rep << std::endl;
			int t5;
			std::vector<std::string> countries;
			std::vector<std::string> contents;
			extractCountriesAndContents(rep, countries, contents);	
			std::cout << "가고 싶은 여행지를 선택해 주세요." << std::endl;
			std::cout << "1. " << countries[0] << " 2. "<< countries[1] << " 3. " << countries[2] <<" 4. 재추천 해주세요."<< std::endl;
			std::cin >> t5;
			if(t5!=4){
				coun = countries[t5-1];
				counc = contents[t5-1];
				break;
			}
			else{
				placeq = countries[0] + ", " + countries[1] + ", " + countries[2] + " 이 나라들 제외하고" + placeq ;
			}
		}
	}
	std::cout << "여행 일정이 며칠인가요?"<<std::endl;
	int t6;
	std::cin >> t6;
	guide trip(t6);
	trip.init(coun, counc);
	std::string rt[6] = {"아침식사", "조간 활동", "점심식사", "주간 활동", "저녁식사", "야간 활동"};
	for(int i = 1; i <= trip.get_day(); i++){
		std::string ho = "";
		for(int j = 0; j <= 6; j++){
			std::string qst = "";
			if(j == 0){
				qst = qst + trip.get_con() + "에서 " + std::to_string(i) + "일날 숙소 3곳 추천해줘" ;
			}
			else{
				qst = qst + trip.get_con() +  " " + ho + "주변에서 " + std::to_string(i) + "일날 " + rt[j-1] + "할만 한 3 곳 추천해줘" ;
			}
			int te = 1;
			while(te){
				std::cout << qst << std::endl;
				std::vector<std::string> cc;
				std::vector<std::string> ccc;
				std::string repl = reply(qst);
				std::cout << repl << std::endl;
				extractCountriesAndContents(repl, cc, ccc);	
				std::cout << "1. " << cc[0] << " 2. "<< cc[1] << " 3. " << cc[2] <<" 4. 재추천 해주세요."<< std::endl;
				int t7;
				std::cin >> t7;
				if(t7!=4){
					te = 0;
					if(j == 0){
						ho = cc[t7-1];
					}
				} 
				else{
					qst = cc[0] + ", " + cc[1] + ", " + cc[2] + " 이 곳을 제외하고" + qst;
				}
			}
		}	
	}		
}

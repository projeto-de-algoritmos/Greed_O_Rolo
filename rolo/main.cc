#include <drogon/drogon.h>

using namespace drogon;

#include <iostream>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <unordered_map>
#include <bits/stdc++.h> 
using namespace std;


#define io                        \
    ios_base::sync_with_stdio(0); \
    cin.tie(0);                   \
    cout.tie(0);

unordered_map<char, string> hashmap;

struct huffnode
{
    char character;
    unsigned int freq;
    struct huffnode *left, *right;
};
typedef struct huffnode *NODE;

struct Minheap
{
    unsigned short size, capacity;
    NODE *array;
};
typedef struct Minheap *HEAP;

NODE newleafnode(char c, int f)
{
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = c;
    tmp->freq = f;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

NODE newinternalnode(int f)
{
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = 0;
    tmp->freq = f;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

void swap(NODE *a, int i, int j)
{
    NODE tmp = (NODE)malloc(sizeof(huffnode));
    tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void heapify(HEAP heap)
{
    int i, j, k;
    unsigned short n = heap->size;
    NODE *a = heap->array;
    for (i = n / 2; i > 0; i--)
    {
        if (2 * i + 1 <= n)
            j = a[2 * i]->freq < a[2 * i + 1]->freq ? 2 * i : 2 * i + 1;
        else
            j = 2 * i;

        if (a[j]->freq < a[i]->freq)
        {
            swap(a, i, j);
            while (j <= n / 2 && (a[j]->freq > a[2 * j]->freq || (2 * j + 1 <= n ? a[j]->freq > a[2 * j + 1]->freq : 0)))
            {
                if (2 * j + 1 <= n)
                    k = a[2 * j]->freq < a[2 * j + 1]->freq ? 2 * j : 2 * j + 1;
                else
                    k = 2 * j;
                swap(a, k, j);
                j = k;
            }
        }
    }
}

NODE mintop(HEAP heap)
{
    NODE top = (NODE)malloc(sizeof(huffnode));
    top = heap->array[1];
    heap->array[1] = heap->array[heap->size--];
    heapify(heap);
    return top;
}

void insertnode(HEAP heap, NODE leftchild, NODE rightchild, int f)
{
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = '\0';
    tmp->freq = f;
    tmp->left = leftchild;
    tmp->right = rightchild;
    heap->array[++heap->size] = tmp;
    heapify(heap);
}

void huffman_tree(HEAP heap)
{
    while (heap->size > 1)
    {
        NODE left_child = mintop(heap);
        NODE right_child = mintop(heap);
        insertnode(heap, left_child, right_child, (left_child->freq + right_child->freq));
    }
}

void assign_codes(NODE root, string str)
{
    string s1 = "", s2 = "";
    if (!root->left && !root->right)
    {
        hashmap[root->character] = str;
    }
    else
    {
        s1 = str + "0";
        s2 = str + "1";
        assign_codes(root->left, s1);
        assign_codes(root->right, s2);
    }
}

void preorder(NODE root)
{
    if (root != NULL)
    {
        cout << root->freq << "\t";
        preorder(root->left);
        preorder(root->right);
    }
}

string Comprime(const char *filee)
{
    /*io if (argc != 2)
    {
        cout << "Invalid cmd line arg. Usage: ./a.out <input file>\n";
        return 1;
    }*/
    FILE *input = fopen(filee, "r");
    if (input == NULL)
    {
        return "Não há nada no arquivo\n";
    }
    string in = filee;
    string filetype = in.substr(in.find(".")+1);
    cout<<filetype<<"\n";
    string out = in.substr(0, in.find(".")) + "compressed.bin";

    FILE *output = fopen(out.c_str(), "wb");
    if (output == NULL)
    {
        return "Erro criando arquivo comprimido\n";
    }
    string file_contents = "";
    char buff[1];
    unsigned short num_of_unique_chars = 0;

    // Escrevendo o tipo do arquivo
    int ftsize = filetype.length();
    char num = ftsize + '0';
    fwrite(&num, 1, 1, output);
    for(int i=0; i<ftsize; i++){
        buff[0] = filetype[i];
        fwrite(buff, 1, 1, output);
    }

    // Pegando a frequência de cada caracter único
    int i, f_size = 0, freq[256] = {0};
    while (fread(buff, 1, 1, input))
    {
        if (!freq[buff[0] - 0])
            num_of_unique_chars++;
        freq[buff[0] - 0]++;
        f_size++;
    }
    fclose(input);
    FILE *input1 = fopen(filee, "r");

    HEAP heap = (HEAP)malloc(sizeof(struct Minheap));

    // Capicidade (numero de caracteres únicos + 1)
    heap->capacity = num_of_unique_chars + 1;
    heap->size = 0;
    heap->array = (NODE *)malloc(heap->capacity * sizeof(struct huffnode));

    for (int i = 0; i < 256; i++)
    {
        if (freq[i])
        {
            heap->size += 1;
            heap->array[heap->size] = newleafnode(i, freq[i]);
        }
    }
    heapify(heap);
    huffman_tree(heap);
    assign_codes(heap->array[heap->size], "");
    unordered_map<char, string>::iterator x;

    for (x = hashmap.begin(); x != hashmap.end(); x++)

        // Convertendo o conteúdo para binário
        while (fread(buff, 1, 1, input))
        {
            file_contents += hashmap[buff[0]];
        }
    // Encontrando o padding para deixar o binário múltiplo de 8
    int padding = 0;
    if (file_contents.length() % 8 != 0)
        padding = 8 - (file_contents.length() % 8);
    for (int i = 0; i < padding; i++)
    {
        file_contents = "0" + file_contents;
    }

    // Escrevendo os caracteres no inicio doa arquivo
    string codes;
    unordered_map<char, string>::iterator iter;
    for (iter = hashmap.begin(); iter != hashmap.end(); iter++)
    {
        codes = "";
        codes += (iter->first + iter->second);
        fwrite(codes.c_str(), codes.length() + 1, 1, output);
    }

    // Escrevendo o padding dado
    char null = '\0';
    char pad = padding + '0';
    fwrite(&null, 1, 1, output);
    fwrite(&pad, 1, 1, output);
    fwrite(&null, 1, 1, output);
    fwrite(&null, 1, 1, output);
    char tmp_bin[9];
    int dec, w;
    for (int j = 0; file_contents[j]; j = j + 8)
    {
        strncpy(tmp_bin, &file_contents[j], 8);
        dec = 0, i = 7;
        while (i > -1)
        {
            w = tmp_bin[i] == '1' ? 1 : 0;
            dec += ((int)pow(2, 7 - i)) * w;
            i--;
        }
        buff[0] = dec;
        fwrite(buff, 1, 1, output);
    }
    free(heap);
    fclose(input1);
    fclose(output);
    return "Compressed File successfully\n";
}

string dectobin(int decimal){
    string s="";
    for(int i = 7; i>=0; i--){
        if(decimal>>i & 1)s += '1';
        else s += '0';
    }
    return s;
}

string Descomprime(const char* filee){
    /*if(argc != 2){
        cout<<"Invalid cmd line arg. Usage: ./a.out <input file>\n";
        return "Arquivo inválido\n";
    }*/
    FILE *input = fopen(filee, "rb");
    int num_of_unique_chars=0;
    if(input == NULL){
        cout<<"Error opening input file\n";
        return "erro ao abrir arquivo de entrada\n";
    }
    string in = filee;
    string out = in.erase(in.size() - 4, in.size() - 1) + ".";
    char buff[1];

    // Lendo a extensão
    fread(buff, 1, 1, input);
    int fsize = buff[0] - '0';
    while(fsize--){
        fread(buff, 1, 1, input);
        out += buff[0];
    }
    
    FILE *output = fopen(out.c_str(), "w");
    if(output == NULL){
        cout<<"Error creating output file\n";
        return "Erro criando arquivo de saída\n";
    }
    unordered_map<string, char> decode_map;
    string s = "";
    int flag=0;

    // Guardando o hasmap para a decodificação
    while(fread(buff, 1, 1, input)){
        if(buff[0] != '\0'){
            flag = 0;
            s += buff[0];
        }
        else{
            num_of_unique_chars++;
            flag++;
            if(flag == 2)break;
            decode_map[s.substr(1, s.length()-1)] = s[0];
            s = "";
        }
    }

    // Lendo e gravando os nulls extras
    fread(buff, 1, 1, input);
    int padding = buff[0] - '0';
    fread(buff, 1, 1, input);  // null
    fread(buff, 1, 1, input);  // null

    // Código binário começa aqui
    int decimal;
    s = "";
    while(fread(buff, 1, 1, input)){
        decimal = buff[0];
        s += dectobin(decimal);
    }
    int start;    
    if(padding == 0){
        s = "0" + s;
        start = 1;
    }
    else start = padding;
    for(int i=1; s[start]!='\0'; i++){
        if(decode_map.find(s.substr(start, i)) != decode_map.end()){
            fwrite(&(decode_map[s.substr(start, i)]), 1, 1, output);
            start += i;
            i = 0;
        }
    }
    unordered_map<string, char>:: iterator iter;
    fclose(input);
    fclose(output);
    return "Descomprimido\n";
}

int main()
{
    app().registerHandler(
        "/",
        [](const HttpRequestPtr &,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            auto resp = HttpResponse::newHttpViewResponse("FileUpload");
            callback(resp);
        });

    app().registerHandler(
        "/upload_endpoint",
        [](const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            MultiPartParser fileUpload;
            if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() != 1)
            {
                auto resp = HttpResponse::newHttpResponse();
                resp->setBody("Adicione apenas um arquivo");
                resp->setStatusCode(k403Forbidden);
                callback(resp);
                return;
            }
            auto &file = fileUpload.getFiles()[0];
            cout << "Extensão: " <<  file.getFileExtension() << '\n';
            if(file.getFileExtension() == "txt"){
                auto md5 = file.getFileName();
                auto resp = HttpResponse::newHttpResponse();
                file.save();
                string caminho = "uploads/";
                caminho += md5;
                string nome = md5.erase(md5.size() - 4, md5.size() - 1);
                string valor = Comprime(caminho.c_str());
                resp->setBody(nome);
                callback(resp);
            } else if(file.getFileExtension() == "bin"){
                auto md5 = file.getFileName();
                auto resp = HttpResponse::newHttpResponse();
                file.save();
                string caminho = "uploads/";
                caminho += md5;
                string nome = md5.erase(md5.size() - 4, md5.size() - 1);
                //nome = nome.substr(0,nome.find("compressed")) + "decompressed";
                string top = Descomprime(caminho.c_str());
                resp->setBody(nome);
                callback(resp);
            }else{
                auto resp = HttpResponse::newHttpResponse();
                resp->setBody("Extensão não suportada!");
                callback(resp);
            }
            
                },
        {Post});

    app().registerHandler(
        "/baixarBin/{}",
        [](const HttpRequestPtr &req,
            std::function<void(const HttpResponsePtr &)> &&callback, string nomeArquivo) {
            string arquivo = "./uploads/" + nomeArquivo + "compressed.bin";
            string arquivoSaida = nomeArquivo + "-menor.bin";
            auto resp = HttpResponse::newFileResponse(arquivo.c_str(), arquivoSaida.c_str(), CT_NONE);
            callback(resp);
        });

    app().registerHandler(
        "/baixarTxt/{}",
        [](const HttpRequestPtr &req,
            std::function<void(const HttpResponsePtr &)> &&callback, string nomeArquivo) {
            string arquivo = "./uploads/" + nomeArquivo + ".txt";
            cout << arquivo << '\n';
            string arquivoSaida = nomeArquivo + ".txt";
            auto resp = HttpResponse::newFileResponse(arquivo.c_str(), arquivoSaida.c_str(), CT_NONE);
            callback(resp);
        });


    //LOG_INFO << "Server running on 127.0.0.1:8848";
    app()
        .setClientMaxBodySize(20 * 2000 * 2000)
        .setUploadPath("./uploads")
        .addListener("127.0.0.1",5055)
        .run();
}
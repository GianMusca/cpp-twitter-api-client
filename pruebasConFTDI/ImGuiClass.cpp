/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "imguiClass.h"
#include <iostream>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define IMAGE_SIZE	150
#define IMAGE_OFFSET_X	25
#define IMAGE_OFFSET_Y	30
#define SELECTABLE_SIZE 200
#define SW 1920
#define SH 1080
#define DEFAULTQ 5
#define DEFAULTS 25


/*******************************************************************************
 * CLASS METHODS DEFINITIONS
 ******************************************************************************/
IMGUI::IMGUI(Model& _m) : 
	display(nullptr), m(_m) 
{
	error = false;
	if (!initAllegro()) 
	{
		error = true;
		return;
	}


	display = al_create_display(SW, SH);
	queue = al_create_event_queue();

	al_register_event_source(queue, al_get_display_event_source(display));
	// Tambien necesita Allegro para recibir eventos del usuario, por lo que hay que instalar 
	// el mouse y el teclado y registrar sus eventos.
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	if (display == nullptr || queue == nullptr)
	{
		error = true;
	}

	// Setup de Dear ImGui
	IMGUI_CHECKVERSION();			// Control de version de Dear ImGui
	ImGui::CreateContext();
	if (!error)
	{
		ImGui_ImplAllegro5_Init(display); // Inicializa Dear ImGui
	}

	ImGui::StyleColorsDark();		// Opcional: cambiar la paleta de colores default por otra
	searchButton = false;
	error = false;
	m.setQuantity(DEFAULTQ);
	user = "@";
}

IMGUI::~IMGUI() 
{
	// Cleanup final
	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();
	if (queue != nullptr)
	{
		al_destroy_event_queue(queue);
	}

	if (display != nullptr)
	{
		al_destroy_display(display);
	}

	al_shutdown_primitives_addon();
	//delete model;
	return;
}

bool IMGUI::getError() { return error; }
ALLEGRO_DISPLAY* IMGUI::getDisplay() { return display; }
ALLEGRO_EVENT_QUEUE* IMGUI::getEvQueue() { return queue; }
//bool IMGUI::getRunning() { return running; }


void IMGUI::resizeDisplay() 
{
	ImGui_ImplAllegro5_InvalidateDeviceObjects();
	al_acknowledge_resize(display);
	ImGui_ImplAllegro5_CreateDeviceObjects();
	return;
}

void IMGUI::graphNewFrame() 
{
	// Inicio el frame. Se realiza una vez por cada pantalla que dibujo.
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();
	//PANTALLA
	printScreen();
	// Rendering
	ImGui::Render();	//No dibuja! Solamente calcula que es lo que tiene que dibujarse
	// Puedo usar funciones de dibujo de Allegro ademas de las de 
	// ImGui.
	// Todo lo que dibuje antes de ImGui_ImplAllegro5_RenderDrawData
	// va a quedar detras de las ventanas de DearImGui, y todo lo 
	// que dibuje despues va a quedar encima de las ventanas de 
	// DearImGui.

	al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));				//Va a quedar detras de las ventanas
	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());	//Dibuja las ventanas, pero no hace al_flip_display()
	//Todo lo que dibuje aca va a quedar por encima de las ventanas de DearImGui

	al_flip_display(); //DearImGui nunca hace al_flip_display()
	return;
}

void IMGUI::printScreen() 
{
	int j = 0;
	ImGui::Begin("LCD Tweet Receiver");
	static char buf1[MAX_LENGHT] = "";
	buf1[0] = '@';
	ImGui::InputText("Enter user:", buf1, MAX_LENGHT);

	ImGui::SameLine();

	ImGui::SameLine();
	if (ImGui::Button("Bring Tweets")) 
	{
		static unsigned int calls = 0;
		string aux(buf1);
		aux.erase(0, 1);

		m.setUsername(aux);
		if (!calls)
		{
			m.setState(CONFIGURATION);
			calls++;
		}

		else
		{
			m.setState(MORE);
			calls++;
		}
	
		ImGui::Begin("Downloading...");
		ImGui::Text("Downlading tweets");
		//Llamado a la clase nueva(?)
		//model->compressImages(); funcion del model
		ImGui::End();
	}

	ImGui::SameLine();
	if (ImGui::Button("Abort"))
	{
		m.setState(FINISHED);
	}


	if (ImGui::Checkbox("Default Tweet Count", &defaultCB)) {
	//	tweetCount = 10;
		m.setQuantity (10);								
	}

	if (!defaultCB)
	{	
		static int auxquant = DEFAULTQ;
		ImGui::SliderInt("Tweets", (int*)&auxquant, 0, 500);
		m.setQuantity(auxquant);
	}

	else
	{
		int defaultC = DEFAULTQ;
		ImGui::SliderInt("Tweets", (int*)& defaultC, 10, 10);
	}

	ImGui::SameLine();
	if (ImGui::Button("Previous")) 
	{
		m.prevTweet();
	}
	ImGui::SameLine();
	if (ImGui::Button("Repeat")) 
	{
		m.repeatTweet();
	}
	ImGui::SameLine();
	if (ImGui::Button("Next")) 
	{
		m.nextTweet();
	}

	ImGui::NewLine();
	if (ImGui::Checkbox("Default Speed", &defaultSp)) 
	{
		auxSpeed = DEFAULTS;
		m.setSpeed(auxSpeed);
	}

	if (!defaultSp)
	{
		ImGui::SliderInt("Speed", (int*)&auxSpeed, 0, 100);
		m.setSpeed(auxSpeed);
	}

	else if(defaultSp)
	{
		int defaultSpeed = DEFAULTS;
		ImGui::SliderInt("Speed", (int*)& defaultSpeed, 25, 25);
	}

	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		auxSpeed = 0;
		m.setSpeed(auxSpeed);
	}


	ImGui::End();
	return;
}


void IMGUI::cycle(void* model) 
{
	ImGuiIO& io = ImGui::GetIO();	// Me permite obtener informacion y cambiar parametros del mouse y de teclado.
									// Por ejemplo, puedo cambiar la velocidad a la que se repiten los caracteres 
									// cuando mantengo una tecla presionada, o puedo saber si el evento de 
									// mouse/teclado que acaba de llegar fue dentro de una ventana de ImGui o fuera.

		ALLEGRO_EVENT ev;
		while (al_get_next_event(queue, &ev))
		{
			ImGui_ImplAllegro5_ProcessEvent(&ev);	// Mandar el evento a Dear ImGui para que lo procese
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				m.setEnd(true);
			}
			if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
			{
				resizeDisplay();
			}
			// io.WantCaptureKeyboard devuelve true si el ultimo evento fue de teclado y ademas fue 
			// usado por el usuario para interactuar con las ventanas de Dear Imgui. Por este motivo, 
			// sabemos que no estaba dirigido a nuestra aplicacion principal y no debemos mandarselo a 
			// su dispatcher. Idem para io.WantCaptureMouse y los eventos de mouse
			if (!io.WantCaptureKeyboard && !io.WantCaptureMouse)
			{
				// Dispatch a nuestra aplicacion principal 
			}

		}
		graphNewFrame();
}

void IMGUI::update(void* model){}

/*******************************************************************************
 * LOCAL FUNCTIONS DEFINITIONS
 ******************************************************************************/
bool IMGUI:: initAllegro(void)
{
	bool ret = true;
	// ---------- Inicializacion de Allegro. ----------
	al_init();

	// Dear ImGui necesita Allegro para poder graficar. Para esto, usa el add-on de primitivas.
	if (!al_init_primitives_addon())
	{
		ret = false;
	}

	// El display usado es el de Allegro. Se registran sus eventos para poder cerrarlo 
	// apretando 'X' o para poder cambiar su tamanio en ejecucion.
	al_set_new_display_flags(ALLEGRO_RESIZABLE);

	if (!al_install_keyboard())
	{
		ret = false;
	}

	if (!al_install_mouse())
	{
		ret = false;
	}

	if (!al_init_image_addon())
	{
		ret = false;
	}

	return ret;
}

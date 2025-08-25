// Ficheiro: js/library_settings.js (com o padrão Singleton)

// VERIFICA se o nosso controlador de página já existe.
if (!window.LibrarySettingsPage) {

    // Se NÃO existir, declara o objeto completo com toda a sua lógica.
    // Este bloco de código só vai correr UMA VEZ.
    window.LibrarySettingsPage = {
        folderInput: null,
        selectButton: null,

        /**
         * Inicializa a página de configurações da biblioteca.
         */
        init: function() {
            console.log("Initializing Library Settings Page...");
            
            // Guarda as referências aos elementos do DOM
            this.folderInput = document.getElementById('selectedFolderInput');
            this.selectButton = document.getElementById('selectFolderBtn');

            // Garante que não adicionamos o mesmo listener várias vezes
            // Primeiro removemos um possível listener antigo e depois adicionamos o novo.
            this.selectButton.removeEventListener('click', this.handleSelectFolder);
            this.selectButton.addEventListener('click', this.handleSelectFolder);

            // Adiciona um "ouvinte" para o evento 'emojilibraryupdated'
            window.addEventListener('emojilibraryupdated', this.updateUI.bind(this));

            // Atualiza a UI com o estado atual do serviço quando a página é carregada
            this.updateUI();
        },

        // Função "handler" para o clique do botão, para facilitar a remoção do listener
        handleSelectFolder: async function() {
            // 'this' aqui dentro seria o botão, por isso usamos window.LibrarySettingsPage
            const success = await window.EmojiLibraryService.selectFolder();
            if (success) {
                window.LibrarySettingsPage.updateUI();
            }
        },

        /**
         * Atualiza a interface do utilizador com o nome da pasta selecionada.
         */
        updateUI: function() {
            // 'this' pode variar dependendo de como a função é chamada,
            // então usamos 'window.LibrarySettingsPage' para segurança, ou .bind(this) no listener.
            const handle = window.EmojiLibraryService.directoryHandle;
            const input = document.getElementById('selectedFolderInput'); // Busca o elemento de novo para garantir
            
            if (input) { // Verifica se o elemento existe na página atual
                if (handle) {
                    input.value = handle.name;
                } else {
                    input.value = '';
                }
            }
        }
    };
}

// PONTO DE ENTRADA: Esta linha corre SEMPRE que a página é carregada.
// Ela chama a função init do nosso objeto já existente para re-desenhar e re-ativar a página.
window.LibrarySettingsPage.init();